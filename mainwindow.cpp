#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configreader.h"
#include "stylehelper.h"
#include "searchwin.h"
#include "dialog.h"
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Читаем конфигурационный файл
    try{
        ConfigReader confReader("default.conf");
    }catch(...){
        qDebug() << "Не удалось прочитать данные из файла конфигурации";
    }
    setInterfaceStyle();

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::ruleChengeButton);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ruleChengeButton()
{
    QString g =ui->pushButton->objectName();
    Dialog dlg(g,this);
    qDebug() << "ff";
    dlg.exec();
    dlg.getGo();


}

void MainWindow::setInterfaceStyle()
{
    StyleHelper::setFonts();
    this->setStyleSheet(StyleHelper::getMainStyleLight()); 
}


void MainWindow::on_action_3_triggered()
{
    searchWin dlg(this);
    dlg.exec();
    if(dlg.back){
        if(dlg.searchOrRepl){
            searchinText = dlg.searchGo();
            qDebug()<<searchinText<<"serch" ;
        }else{
            searchinText = dlg.searchAndRepGo();
            replaceText = dlg.replaceGo();
            qDebug()<<searchinText<<"rep"<<replaceText ;

        }
    }
}

