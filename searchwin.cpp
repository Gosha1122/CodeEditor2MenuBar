#include "searchwin.h"
#include "ui_searchwin.h"

searchWin::searchWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchWin)
{
    ui->setupUi(this);
}

searchWin::~searchWin()
{
    delete ui;
}

QString searchWin::searchGo()
{
    return ui->lineEdit->text();
}
