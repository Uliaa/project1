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
QSqlDatabase db;
QString FileWay2;
QString FileWay1;
QFile file;

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
           QString truestr = "CREATE TABLE addressbook ( "
                         "number INTEGER PRIMARY KEY NOT NULL, "
                         "name VARCHAR(15), " "phone VARCHAR(12), "
                         "email VARCHAR(15) "
                   ");";
           QString str1 = " ( "
                         "number INTEGER PRIMARY KEY NOT NULL, "
                         "name VARCHAR(15), " "phone VARCHAR(12), "
                         "email VARCHAR(15) "
                     ");";
           QFileInfo fi(FileWay1);

          QString  str="CREATE TABLE "+fi.baseName()+str1;
           qDebug() << str;
           qDebug() << truestr;

      /*  QSqlQuery query;
        QString str = "CREATE TABLE addressbook ( "
                      "number INTEGER PRIMARY KEY NOT NULL, "
                      "name VARCHAR(15), " "phone VARCHAR(12), "
                      "email VARCHAR(15) "
                ");";*/
        if (!query.exec(str)) {
            qDebug() << "Unable to create a table";
        }
        // Добавляем данные в базу
        QString strF = "INSERT INTO addressbook (number, name, phone, email) "
                       "VALUES(%1, '%2', '%3', '%4');";

        str = strF.arg("1")
                       .arg("Piggy")
                       .arg("+49 631322187")
                       .arg("piggy@mega.de");
        if (!query.exec(str)) {
            qDebug() << "Unable to make insert operation";
        }
        str = strF.arg("2")
                .arg("Kermit")
                .arg("+49 631322181")
                .arg("kermit@mega.de");
        if (!query.exec(str)) {

            qDebug() << "Unable to make insert operation";
        }
        if (!query.exec("SELECT * FROM addressbook;")) {

            qDebug() << "Unable to execute query - exiting";
        }
     }}

        /*
         QString head;
         QStringList::const_iterator it;
         // Создаём поток для извлечения данных из файла
         QTextStream in(&file);
         // Считываем данные до конца файла
         //while (!in.atEnd())
         QString line = in.readLine();
         QString str = "CREATE TABLE" + FileWay1+ "(\" ";
                 for (QString it : line.split(";")) {
                  head=""+it+""+typeid(it).name ();
                  str.append("\""+head+",\"");
                 }
                 str.truncate(str.length()-2);
                 str.append("\"\");\";");
         if (!query.exec(str)) {
             qDebug() << "Unable to create a table";
         }
         // Добавляем данные в базу
         QString strF =
                 "INSERT INTO"+FileWay1 +" (";
                  for (QString it : line.split(";")) {
                    strF.append(it+",");}
                    strF.truncate(str.length()-1);
                    strF.append("\"");
                               //  strF.append("" \"VALUES(");
                 //"VALUES(%1, '%2', '%3', '%4');";
      //   str = strF.arg("Piggy")
      //             .arg("+49 631322187")
      //             .arg("piggy@mega.de");
     //            if (!query.exec(str)) {
     //            qDebug() << "Unable to make insert operation";

         //str = strF.arg("2")
          //       .arg("Kermit")
          //       .arg("+49 631322181")
          //       .arg("kermit@mega.de");
        // if (!query.exec(str)) {
        //     qDebug() << "Unable to make insert operation";
        // }
        // if (!query.exec("SELECT * FROM addressbook;")) {
        //     qDebug() << "Unable to execute query - exiting";
        // }*/



