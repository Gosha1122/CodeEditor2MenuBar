#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>


Dialog::Dialog(QString text,QString format,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle (text );
    ui->listWidget->addItem("C++");
    ui->listWidget->addItem("JavaScript");
    ui->listWidget->addItem("HTML");
    ui->listWidget->addItem("CSS");
    ui->listWidget->addItem("PlainText");
    ui->listWidget->addItem("QSS");
    connect(ui->listWidget,&QListWidget::itemClicked,this,&Dialog::elListClickSlot);

    for(int i=0;i<ui->listWidget->count();i++){
        if(format == ui->listWidget->item(i)->text() ){
            ui->listWidget->setCurrentItem(ui->listWidget->item(i));
            qDebug()<<"talalala";
        }
        qDebug()<<i;
    }



}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getGo()
{

    go.append(ui->listWidget->currentItem()->text());
    qDebug()<<go;
    return go;
}

void Dialog::elListClickSlot()
{

    index=ui->listWidget->currentRow();

    this->accept();
}


