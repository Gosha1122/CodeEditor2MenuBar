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
    QString searchAndRepGo();
    QString replaceGo();

    void searchButton();
    void searchAndRepButton();
    void backButton();

    bool back;
    bool searchOrRepl;
private:
    Ui::searchWin *ui;
};

#endif // SEARCHWIN_H
