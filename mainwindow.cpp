#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include "QStandardItemModel"
#include "QStandardItem"
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
}

void MainWindow::on_toolButton_clicked()
{       FileWay1=QFileDialog::getOpenFileName();
        ui->lineEdit->setText(FileWay1);
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1){

    QString TableName=ui->comboBox->currentText();
    model=new QSqlTableModel(this,db);
    model->setTable(TableName);
    model->select();
    ui->tableView->setModel(model);

}


void MainWindow::on_Export_clicked()
{ if(this->ui->SQlite_CSV->isChecked()){
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
        if(this->ui->CSV_SQlite->isChecked()){


           QStringList StoredData;
           QString SqlQueryString;
           QString DataType="INTEGER";
           QString TextValue;
           QString AllHeaders;
           QString StringData;
           QString FileWay=ui->lineEdit_2->text();
           db=QSqlDatabase::addDatabase("QSQLITE");
           db.setDatabaseName(FileWay);
           db.open();
           QSqlQuery query;
           QFileInfo fi(FileWay1);


           for( int i = 0; i < ui->tableView->horizontalHeader()->count(); ++i )
               {
                   QString HeaderNames = ui->tableView->model()->headerData(i, Qt::Horizontal).toString();
                   SqlQueryString+=" "+HeaderNames;
                   AllHeaders+=" "+HeaderNames+",";
                   for( int j = 0; j < ui->tableView->verticalHeader()->count(); ++j )
                   {
                     TextValue = ui->tableView->model()->data(ui->tableView->model()->index(j, i)).toString();


                     bool success;
                     int ConvertData =TextValue.toInt(&success);
                     if((success)&&(DataType=="INTEGER")){
                         DataType="INTEGER";
                     } else{
                         float CovertData1=TextValue.toDouble(&success);
                         if (success){
                                 DataType="REAL";
                     }else {

                         DataType="TEXT";
                                 break;
                        }
                     }}
                   SqlQueryString+=" "+DataType+" ,";

                   DataType="INTEGER";
                   }
           SqlQueryString.remove(SqlQueryString.size()-1,1);
           AllHeaders.remove(AllHeaders.size()-1,1);
           query.prepare("CREATE TABLE "+fi.baseName()+"("+SqlQueryString+");");
            qDebug()<<"CREATE TABLE "+fi.baseName()+"("+SqlQueryString+");";
           query.exec();
           for( int j = 0; j < ui->tableView->verticalHeader()->count(); ++j )
           {
               for( int i = 0; i < ui->tableView->horizontalHeader()->count(); ++i )
               {
                   TextValue = ui->tableView->model()->data(ui->tableView->model()->index(j, i)).toString();
                   StringData+="'"+TextValue+"' ,";
               }
               StringData.remove(StringData.size()-1,1);

               query.prepare("INSERT INTO "+fi.baseName()+"("+AllHeaders+") VALUES("+StringData+");");
               query.exec();
               StringData="";
           }

           db.close();
    }

}

void MainWindow::on_CSV_SQlite_clicked()
{  QFile file(FileWay1);
    QStandardItemModel *model = new QStandardItemModel;
   QStandardItem *item;
   QStringList horizontalHeader;
   int i,j;
   i=-1; j=0;

if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
qDebug() <<"File not exists";
}
else{
QStringList::const_iterator it;
// Создаём поток для извлечения данных из файла
QTextStream in(&file);
// Считываем данные до конца файла
int flag=0;
while (!in.atEnd())
{
// ... построчно
QString line = in.readLine();

for (QString it : line.split(";")) {
if (flag==0){
    horizontalHeader.append(it);
    model->setHorizontalHeaderLabels(horizontalHeader);
    qDebug()<<it;
}
else{
item = new QStandardItem(it);
model->setItem(i, j, item);
j++;
}
}
j=0;
i++;
flag++;
 file.close();
 ui->tableView->setModel(model);
 ui->tableView->resizeRowsToContents();
 ui->tableView->resizeColumnsToContents();
 }}

}

void MainWindow::on_SQlite_CSV_clicked()
{     db=QSqlDatabase::addDatabase("QSQLITE");
      db.setDatabaseName(FileWay2);
      db.open();
      ui->comboBox->clear();
      QStringList TablesName=db.tables();
      for(int i=0;i<TablesName.size(); i++){
          ui->comboBox->addItem(TablesName[i]);
      }

}
