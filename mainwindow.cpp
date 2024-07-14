#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configreader.h"
#include "stylehelper.h"
#include "searchwin.h"
#include "dialog.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
//fffff
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

    ui->tabWidget->setStyleSheet(StyleHelper::StyleHelperTabWidget(1));
    ui->tabWidget_2->setStyleSheet(StyleHelper::StyleHelperTabWidget(1));

    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->addTab(new CodeEditor, "untitled");
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(ui->tabWidget->count() - 1));
    connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    ui->tabWidget->addTab(new CodeEditor, "untitled1");
    editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(ui->tabWidget->count() - 1));
    connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::ruleChengeButton);


    ui->tabWidget->tabBar()->installEventFilter(this);


    setupMenuBar();
    treeFilesWidget = new TreeFilesWidget;
    QVBoxLayout* box = new QVBoxLayout;
    ui->leftWidget->setLayout(box);
    box->addWidget(treeFilesWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->tabWidget->tabBar() ) {

        if  (event->type() == QEvent::HoverLeave) {
            qDebug()<<"Мышка ушла";
            ui->tabWidget->setStyleSheet(StyleHelper::StyleHelperTabWidget(1));

        } else if (event->type() == QEvent::HoverEnter) {

            qDebug()<<"Мышка пришла";
            ui->tabWidget->setStyleSheet(StyleHelper::StyleHelperTabWidget(0));
        }
    }
    return QWidget::eventFilter(target, event);
}

//Переключение стилей.
void MainWindow::ruleChengeButton()
{
    QString g =ui->pushButton->objectName();
    QString n =ui->pushButton->text();
    Dialog dlg(g,n,this);
    dlg.exec();
    QString name =dlg.getGo();
    qDebug() << name;
    ui->pushButton->setText(name);
    if(name=="C++")
        addCppColorMenu();
    else if(name=="JSON")
        addJSONColorMenu();
    else if(name=="CSS")
        addQSSColorMenu();
    else if(name=="QSS")
        addQSSColorMenu();



}

void MainWindow::NewFileMenu()
{
    ui->tabWidget->addTab(new CodeEditor, "untitled" + QString::number(MainWindow::count));
    MainWindow::count ++;
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(ui->tabWidget->count() - 1));
    connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::OpenFileMenu()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*.*)");
    if(!filepath.isEmpty()){
        QFileInfo info(filepath);
        QString filename = info.fileName();
        QFile file(filepath);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            ui->tabWidget->addTab(new CodeEditor, filename);
            QString text = file.readAll();
            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
            QString suf = info.suffix();
            CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
            connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
            editor->path = filepath;
            editor->appendPlainText(text);
            if(suf == "cpp" || suf == "h" || suf == "c" || suf == "hpp"){
                if(editor->highlighter != nullptr){
                    delete editor->highlighter;
                }
                editor->highlighter = new Highlighter(editor->document());
            }else if(suf == "json"){
                if(editor->highlighter != nullptr){
                    delete editor->highlighter;
                }
                editor->highlighter = new HighlighterJSON(editor->document());
            }else if(suf == "QSS"){
                if(editor->highlighter != nullptr){
                    delete editor->highlighter;
                }
                editor->highlighter = new HighlighterQSS(editor->document());
            }else{
                if(editor->highlighter != nullptr){
                    delete editor->highlighter;
                }
                editor->highlighter = nullptr;
            }
            editor->save = false;
            editor->status = false;
            file.close();
        }
    }
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

void MainWindow::closeFileMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(!editor->save){
        QMessageBox::Button msg;
        msg = QMessageBox::warning(this,"Закрыть файл", "Cохранить файл перед закрытием?", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok);
        if(msg == QMessageBox::Ok){
            MainWindow::SaveFileMenu();
        }
    }
    delete editor;
    editor = nullptr;
}

void MainWindow::SaveFileMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(!editor->save){
        if(!editor->status){
            QString filepath = QFileDialog::getSaveFileName(this,"Save", "", "");
            if(filepath.isEmpty())
                return;
            QFile file(filepath);
            QString text = editor->document()->toPlainText();
            file.open(QIODevice::WriteOnly|QIODevice::Text);
            file.write(text.toUtf8());
            file.close();

            QFileInfo inf(filepath);
            QString filename = inf.fileName();
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), filename);

            editor->save = true;
            editor->status = true;
            editor->path = filepath;
        }else{
            QFile file(editor->path);
            QString text = editor->document()->toPlainText();
            file.open(QIODevice::WriteOnly|QIODevice::Text);
            file.write(text.toUtf8());
            file.close();

        }
    }
}

void MainWindow::SaveAtFileMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    QString filepath = QFileDialog::getSaveFileName(this,"Save", "", "");
    QFile file(filepath);
    QString text = editor->document()->toPlainText();
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    file.write(text.toUtf8());
    file.close();
    editor->save = true;
    editor->path = filepath;
}

void MainWindow::SaveAllFileMenu()
{
    int index = ui->tabWidget->currentIndex();
    int count = ui->tabWidget->count();
    for(int i = 0; i < count; i++){
        ui->tabWidget->setCurrentIndex(i);
        MainWindow::SaveFileMenu();
    }
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::closePrMenu()
{

}

void MainWindow::UndoMenu()
{

}

void MainWindow::RedoMenu()
{

}

void MainWindow::CutMenu()
{

}

void MainWindow::CopyMenu()
{

}

void MainWindow::PastMenu()
{

}

void MainWindow::SelectAllMenu()
{

}

void MainWindow::addCppColorMenu()
{
    qDebug() << "addCppColorMenu";
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor->highlighter != nullptr){
        delete editor->highlighter;
    }
    editor->highlighter = new Highlighter(editor->document());
}

void MainWindow::addJSONColorMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor->highlighter != nullptr){
        delete editor->highlighter;
    }
    editor->highlighter = new HighlighterJSON(editor->document());
}

void MainWindow::addQSSColorMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor->highlighter != nullptr){
        delete editor->highlighter;
    }
    editor->highlighter = new HighlighterQSS(editor->document());
}

void MainWindow::addPlainTextColorMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor->highlighter != nullptr){
        delete editor->highlighter;
    }
    editor->highlighter = nullptr;
}

void MainWindow::setupMenuBar()
{
    QMenu* File = new QMenu(tr("Файл"), this);
    ui->menubar->addMenu(File);
    File->addAction(tr("Новый"), this, &MainWindow::NewFileMenu);
    File->addAction(tr("Открыть"), this, &MainWindow::OpenFileMenu);
    File->addAction(tr("Закрыть"), this, &MainWindow::closeFileMenu);
    File->addAction(tr("Сохранить"), this, &MainWindow::SaveFileMenu);
    File->addAction(tr("Сохранить как"), this, &MainWindow::SaveAtFileMenu);
    File->addAction(tr("Сохранить всё"), this, &MainWindow::SaveAllFileMenu);
    File->addAction(tr("Выйти"), this, &MainWindow::closePrMenu);
    QMenu* Correction = new QMenu(tr("Редактировать"), this);
    ui->menubar->addMenu(Correction);
    Correction->addAction(tr("Отмена"), this, &MainWindow::UndoMenu);
    Correction->addAction(tr("Вернуть"), this, &MainWindow::RedoMenu);
    Correction->addAction(tr("Вырезать"), this, &MainWindow::CutMenu);
    Correction->addAction(tr("Копировать"), this, &MainWindow::CopyMenu);
    Correction->addAction(tr("Вставить"), this, &MainWindow::PastMenu);
    Correction->addAction(tr("Выделить всё"), this, &MainWindow::SelectAllMenu);
    QMenu* Help = new QMenu(tr("Помощь"), this);
    ui->menubar->addMenu(Help);
    Help->addAction(tr("Помощь"), this, &MainWindow::HelpHelpMenu);
    Help->addAction(tr("Помощь от Qt"), this, &MainWindow::HelpQtHelpMenu);
    QMenu* Color = new QMenu(tr("Подсветка"), this);
    ui->menubar->addMenu(Color);
    Color->addAction(tr("C++"), this, &MainWindow::addCppColorMenu);
    Color->addAction(tr("JSON"), this, &MainWindow::addJSONColorMenu);
    Color->addAction(tr("QSS"), this, &MainWindow::addQSSColorMenu);
    Color->addAction(tr("PlainText"), this, &MainWindow::addPlainTextColorMenu);
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(index));
    if(!editor->save){
        QMessageBox::Button msg;
        msg = QMessageBox::warning(this,"Закрыть файл", "Cохранить файл перед закрытием?", QMessageBox::Ok|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Ok);
        if(msg == QMessageBox::Ok){
            ui->tabWidget->setCurrentIndex(index);
            MainWindow::SaveFileMenu();
        }else if(msg == QMessageBox::No){
        }else{
            return;
        }
    }
    ui->tabWidget->removeTab(index);
    delete editor;
    editor = nullptr;
}

void MainWindow::setSave()
{
    if(ui->tabWidget->tabText(ui->tabWidget->currentIndex())[ui->tabWidget->tabText(ui->tabWidget->currentIndex()).length() - 1] != "*"){
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), ui->tabWidget->tabText(ui->tabWidget->currentIndex()) + "*");
    }
}

void MainWindow::addZVTabWidget(int index)
{
    if(ui->tabWidget->tabText(index)[ui->tabWidget->tabText(index).length() - 1] == '*'){
        ui->tabWidget->setTabText(index, ui->tabWidget->tabText(index).chopped(ui->tabWidget->tabText(index).length() - 2));
    }

}



