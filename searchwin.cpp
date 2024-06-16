#include "searchwin.h"
#include "ui_searchwin.h"

searchWin::searchWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchWin)
{

    ui->setupUi(this);
    QDialog::setWindowTitle ("Поиск" );
    back=false;
    connect(ui->serchButton,&QPushButton::clicked,this,&searchWin::searchButton);
    connect(ui->serchAndRepButton,&QPushButton::clicked,this,&searchWin::searchAndRepButton);
    connect(ui->pushButton,&QPushButton::clicked,this,&searchWin::backButton);
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
    back=true;
    searchOrRepl=true;
    this->accept();
}

void searchWin::searchAndRepButton()
{
    back=true;
    searchOrRepl=false;
    this->accept();
}
void searchWin::backButton()
{
    back=false;
    this->accept();
}
