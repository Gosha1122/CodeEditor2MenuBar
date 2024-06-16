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

void MainWindow::searchButtonClicked()
{
    searchWin dlg(this);
    dlg.exec();
    if(dlg.CopyOrRepl){
        searchinText = dlg.searchGo();
        qDebug()<<"cop",searchinText;
    }else{
        searchinText = dlg.searchAndRepGo();
        qDebug()<<"rep",searchinText;
    }
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

