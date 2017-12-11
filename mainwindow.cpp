#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include "QStandardItemModel"
#include "QStandardItem"
//01.11
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QSqlRecord>
#include <QSqlError>
QSqlDatabase db;
QString FileWay2;
QString FileWay1;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_2_clicked()
{       db.close();
    FileWay2=QFileDialog::getOpenFileName();
     ui->lineEdit_2->setText(FileWay2);
         db=QSqlDatabase::addDatabase("QSQLITE");
         db.setDatabaseName(FileWay2);
         db.open();
         ui->comboBox->clear();
         QStringList TablesName=db.tables();
         for(int i=0;i<TablesName.size(); i++){
             ui->comboBox->addItem(TablesName[i]);

         }
}

void MainWindow::on_toolButton_clicked()
{     QStandardItemModel *model = new QStandardItemModel;
      QStandardItem *item;
      int i,j;
      i=0; j=0;
    FileWay1=QFileDialog::getOpenFileName();
    ui->lineEdit->setText(FileWay1);
    QFile file(FileWay1);
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
    qDebug() <<"File not exists";
    } else {
    QStringList::const_iterator it;
    // Создаём поток для извлечения данных из файла
    QTextStream in(&file);
    // Считываем данные до конца файла
    while (!in.atEnd())
    {
    // ... построчно
    QString line = in.readLine();
    //QStringList list=line.split(";");
    for (QString it : line.split(";")) {
    //for (it = list.constBegin(); it != list.constEnd(); ++it){
    item = new QStandardItem(it);
    model->setItem(i, j, item);
    j++;
    }
    j=0;
    i++;
    }
    file.close();
    }



  ui->tableView->setModel(model);

  ui->tableView->resizeRowsToContents();
  ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1){

    QString TableName=ui->comboBox->currentText();
    model=new QSqlTableModel(this,db);
    model->setTable(TableName);
    model->select();
    ui->tableView->setModel(model);

}

void MainWindow::on_pushButton_clicked()
{    if(this->ui->radioButton_2->isChecked()){
    QString filename=QFileDialog::getSaveFileName(); //открытие диалогового окна и ввод имени файла
    QFile filecsv(filename+".csv"); //создание файла с расширение csv
        if (filecsv.open(QIODevice::WriteOnly)) //если открылся, то...
        {
            QTextStream tex(&filecsv); //создание текстового потока
            QStringList List; //создание строкового списка
            List << "\"\"";
            for (int j = 0; j< ui->tableView->horizontalHeader()->count(); ++j) //проход цикла по колонкам таблицы
                List<<"\""+ui->tableView->model()->headerData(j,Qt::Horizontal).toString()+"\""; //создание шапки таблицы
            tex << List.join(";")+"\n";
            for (int i = 0; i< ui->tableView->verticalHeader()->count();++i)
            {   List.clear();
                List <<"\""+ui->tableView->model()->headerData(i, Qt::Vertical).toString()+"\"";
                for(int j=0; j<ui->tableView->horizontalHeader()->count();++j)
                {
                List <<"\""+ui->tableView->model()->data(ui->tableView->model()->index(i,j),Qt::DisplayRole).toString()+"\"";
            }
            tex << List.join(";")+"\n";
        }
        filecsv.close();
        }
}
     if(this->ui->radioButton->isChecked()){
           QSqlQuery query;
           QString str1=" ( ";
           QString str2=" ( ";
           QString str3="   VALUES(";
           QString s;
           QFile file(FileWay1);
           if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
           qDebug() <<"File not exists";
           }
           else {
           QTextStream in(&file);
           QString line = in.readLine();
           int i=1;
           for (QString it : line.split(";")) {
              str1=str1+it+" TEXT"+",";
              str2=str2+it+",";
              str3=str3+" :"+s.setNum(i)+",";
              i++;
           }
           str1.truncate(str1.length()-1);
           str1.append(");");
           str2.truncate(str2.length()-1);
           str2.append(" )");
           str3.truncate(str3.length()-1);
           str3.append(")");
           QFileInfo fi(FileWay1);
          query.prepare("CREATE TABLE "+fi.baseName()+str1);
          if( !query.exec() ) {
              qDebug() << db.lastError().text();
          }
        // Добавляем данные в базу
        query.prepare(
                    "INSERT INTO "+ fi.baseName()+str2+
                    str3
                    );
        int j=0;
        while (!in.atEnd()){
        line = in.readLine();
        for (QString it : line.split(";")) {
        qDebug()<< it;
        j++;
        qDebug()<< j;
        query.bindValue( ":"+s.setNum(j), it);
        qDebug()<< line;
        }
        j=0;
           //}
       if( !query.exec() ) {
            qDebug() << db.lastError().text();
        }

     }}}}





