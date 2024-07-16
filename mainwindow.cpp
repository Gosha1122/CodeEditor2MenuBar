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
#include <QTextCodec>

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
    ui->tabWidget->addTab(new CodeEditor, "untitled ");
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(ui->tabWidget->count() - 1));
    connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    ui->tabWidget->addTab(new CodeEditor, "untitled1 ");
    editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(ui->tabWidget->count() - 1));
    connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::ruleChengeButton);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::setCodirLabel);

    ui->tabWidget->tabBar()->installEventFilter(this);
    ui->pushButton_2->setText("UTF8");

    setupMenuBar();
    treeFilesWidget = new TreeFilesWidget;
    QVBoxLayout* box = new QVBoxLayout;
    ui->leftWidget->setLayout(box);
    box->setContentsMargins(0,0,0,0);
    box->addWidget(treeFilesWidget);

    ui->tabWidget_2->hide();
    ui->fileInfoLabel->clear();
    ui->pathLabel->clear();
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
    ui->tabWidget->addTab(new CodeEditor, "untitled" + QString::number(MainWindow::count) + " ");
    MainWindow::count ++;
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
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

            QTextCodec *codec = QTextCodec::codecForName("UTF8");
            QByteArray encodedString = codec->fromUnicode(text);


            ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
            QString suf = info.suffix();
            CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
            connect(editor, &CodeEditor::setSavePered, this, &MainWindow::setSave);
            editor->path = filepath;
            editor->setPlainText(encodedString);
            editor->encodedStr = encodedString;
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
            setFileInfo(filepath);
            setFilePathLabel(filepath);
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

void MainWindow::setFileInfo(QString fname)
{
    QFileInfo info(fname);
    int size = info.size();
    int d = size % 10;
    QString byteWord;
    if(size > 11 && size <= 20){
        byteWord = " байтов";
    }else if(d == 1){
        byteWord = " байт";
    }else if(d > 1 && d < 5){
        byteWord = " байта";
    }else{
        byteWord = " байтов";
    }
    ui->fileInfoLabel->setText(QString::number(size) + byteWord);
}

void MainWindow::setFilePathLabel(QString fname)
{
    if(fname.size() > 100){
        int pos  = fname.size() / 2;
        int len = fname.size() - 100;
        fname.replace(pos,len,"...");
    }
    ui->pathLabel->setText(fname);
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
            QTextCodec *codec;
            if(editor->codir == "UTF8"){
                codec = QTextCodec::codecForName("UTF8");
            }else if(editor->codir == "IBM866"){
                codec = QTextCodec::codecForName("IBM866");
            }else{
                codec = QTextCodec::codecForName("Windows-1251");
            }
            QByteArray encodedString = codec->fromUnicode(text);
            editor->encodedStr = encodedString;

            file.open(QIODevice::WriteOnly|QIODevice::Text);
            file.write(encodedString);
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

            QTextCodec *codec;
            if(editor->codir == "UTF8"){
                codec = QTextCodec::codecForName("UTF8");
            }else if(editor->codir == "IBM866"){
                codec = QTextCodec::codecForName("IBM866");
            }else{
                codec = QTextCodec::codecForName("Windows-1251");
            }
            QByteArray encodedString = codec->fromUnicode(text);
            editor->encodedStr = encodedString;

            file.open(QIODevice::WriteOnly|QIODevice::Text);
            file.write(encodedString);
            file.close();
            MainWindow::addZVTabWidget(ui->tabWidget->currentIndex());
        }
    }
}

void MainWindow::SaveAtFileMenu()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    QString filepath = QFileDialog::getSaveFileName(this,"Save", "", "");
    QFile file(filepath);
    QString text = editor->document()->toPlainText();

    QTextCodec *codec;
    if(editor->codir == "UTF8"){
        codec = QTextCodec::codecForName("UTF8");
    }else if(editor->codir == "IBM866"){
        codec = QTextCodec::codecForName("IBM866");
    }else{
        codec = QTextCodec::codecForName("Windows-1251");
    }
    QByteArray encodedString = codec->fromUnicode(text);
    editor->encodedStr = encodedString;

    file.open(QIODevice::WriteOnly|QIODevice::Text);
    file.write(encodedString);
    file.close();

    QFileInfo inf(filepath);
    QString filename = inf.fileName();
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), filename);

    editor->save = true;
    editor->status = true;
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
    QMenu* Codir = new QMenu(tr("Кодировка"), this);
    ui->menubar->addMenu(Codir);
    Codir->addAction(tr("UTF8"), this, &MainWindow::setCoderMenuBarUTF8);
    Codir->addAction(tr("IBM866"), this, &MainWindow::setCoderMenuBarIBM866);
    Codir->addAction(tr("Windows-1251"), this, &MainWindow::setCoderMenuBarWindows_1251);
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
        QString text = ui->tabWidget->tabText(index);
        text[text.length() - 1] = ' ';
        ui->tabWidget->setTabText(index, text);
    }

}

void MainWindow::setCoderMenuBarUTF8()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    editor->codir = "UTF8";
    ui->pushButton_2->setText(editor->codir);
    QString text = editor->document()->toPlainText();
    QTextCodec *codec;
    codec = QTextCodec::codecForName("UTF8");
    QByteArray encodedString;
    if(!editor->save){
        encodedString = codec->fromUnicode(editor->document()->toPlainText());
    }else{
        encodedString = codec->fromUnicode(editor->encodedStr);
    }
    editor->setPlainText(encodedString);
    editor->save = true;
}

void MainWindow::setCoderMenuBarIBM866()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    editor->codir = "IBM866";
    ui->pushButton_2->setText(editor->codir);
    QString text = editor->document()->toPlainText();
    QTextCodec *codec;
    codec = QTextCodec::codecForName("IBM866");
    QByteArray encodedString;
    if(!editor->save){
        encodedString = codec->fromUnicode(editor->document()->toPlainText());
    }else{
        encodedString = codec->fromUnicode(editor->encodedStr);
    }
    editor->setPlainText(encodedString);
    editor->save = true;
    MainWindow::addZVTabWidget(ui->tabWidget->currentIndex());
}

void MainWindow::setCoderMenuBarWindows_1251()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    editor->codir = "Windows-1251";
    ui->pushButton_2->setText(editor->codir);
    QTextCodec *codec;
    QByteArray encodedString;
    codec = QTextCodec::codecForName("Windows-1251");
    if(!editor->save){
        encodedString = codec->fromUnicode(editor->document()->toPlainText());
    }else{
        encodedString = codec->fromUnicode(editor->encodedStr);
    }
    editor->setPlainText(encodedString);
    editor->save = true;
}

void MainWindow::setCodirLabel(int index)
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->widget(index));
    ui->pushButton_2->setText(editor->codir);
}
