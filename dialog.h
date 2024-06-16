#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QString text,QWidget *parent = nullptr);
    ~Dialog();
    int index;
    QString getGo();
    QString go;
private slots:
    void elListClickSlot();

private:
    Ui::Dialog *ui;


};

#endif // DIALOG_H
