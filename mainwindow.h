#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "highlighter.h"
#include "highlighterQss.h"
#include "highlighterJson.h"

#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void ruleChengeButton();
    void NewFileMenu();
    void OpenFileMenu();
    void SaveCorrectionMenu();
    void SaveAsCorrectionMenu();
    void HelpHelpMenu();
    void HelpQtHelpMenu();
    void on_action_3_triggered();

private:
    Ui::MainWindow *ui;
    void setupMenuBar();
    QString searchinText;
    void setInterfaceStyle();
};
#endif // MAINWINDOW_H
