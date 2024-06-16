#ifndef SEARCHWIN_H
#define SEARCHWIN_H

#include <QDialog>

namespace Ui {
class searchWin;
}

class searchWin : public QDialog
{
    Q_OBJECT

public:
    explicit searchWin(QWidget *parent = nullptr);
    ~searchWin();
    QString searchGo();
private:
    Ui::searchWin *ui;
};

#endif // SEARCHWIN_H
