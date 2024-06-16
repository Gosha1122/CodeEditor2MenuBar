#include "searchwin.h"
#include "ui_searchwin.h"

searchWin::searchWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchWin)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&searchWin::searchButton);
    connect(ui->pushButton,&QPushButton::clicked,this,&searchWin::searchAndRepButton);
}

searchWin::~searchWin()
{
    delete ui;
}

QString searchWin::searchGo()
{
    return ui->lineEdit->text();
}

QString searchWin::searchAndRepGo()
{
    return ui->lineEdit->text();
}

void searchWin::searchButton()
{
    CopyOrRepl=true;
}

void searchWin::searchAndRepButton()
{
    CopyOrRepl=false;
}

