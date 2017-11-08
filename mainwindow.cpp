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
QSqlDatabase db;
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
{
    QString FileWay2=QFileDialog::getOpenFileName();
     ui->lineEdit_2->setText(FileWay2);
    // QSqlDatabase db;
         db=QSqlDatabase::addDatabase("QSQLITE");
         db.setDatabaseName(FileWay2);
         db.open();

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
    QString FileWay1=QFileDialog::getOpenFileName();
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
