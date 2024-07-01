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

    setupMenuBar();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ruleChengeButton()
{

    QString g =ui->pushButton->objectName();
    QString n =ui->pushButton->text();
    Dialog dlg(g,n,this);
    qDebug() << "ff";


    dlg.exec();
    QString name =dlg.getGo();

    qDebug()<<"f";
    ui->pushButton->setText(name);


}

void MainWindow::NewFileMenu()
{

}

void MainWindow::OpenFileMenu()
{

}

void MainWindow::SaveCorrectionMenu()
{

}

void MainWindow::SaveAsCorrectionMenu()
{

}

void MainWindow::HelpHelpMenu()
{

}

void MainWindow::HelpQtHelpMenu()
{

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

void MainWindow::setupMenuBar()
{
    QMenu* File = new QMenu(tr("Файл"), this);
    ui->menubar->addMenu(File);
    File->addAction(tr("Новый"), this, &MainWindow::NewFileMenu);
    File->addAction(tr("Открыть"), this, &MainWindow::OpenFileMenu);
    QMenu* Correction = new QMenu(tr("Редактировать"), this);
    ui->menubar->addMenu(Correction);
    Correction->addAction(tr("Сохранить"), this, &MainWindow::SaveCorrectionMenu);
    Correction->addAction(tr("Сохранить как"), this, &MainWindow::SaveAsCorrectionMenu);
    QMenu* Help = new QMenu(tr("Помощь"), this);
    ui->menubar->addMenu(Help);
    Help->addAction(tr("Помощь"), this, &MainWindow::HelpHelpMenu);
    Help->addAction(tr("Помощь от Qt"), this, &MainWindow::HelpQtHelpMenu);
}

