#include "treefileswidget.h"
#include "ui_treefileswidget.h"
#include <QDebug>
#include <QMenu>
#include <QDir>
#include <QFileInfoList>

TreeFilesWidget::TreeFilesWidget(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::TreeFilesWidget)
    , currentListItem(nullptr)
    , currentTreeItem(nullptr)
{
    ui->setupUi(this);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &TreeFilesWidget::customContextMenuRequested, this, &TreeFilesWidget::customContextMenuRequestedSlot);

    // Путь для примера
    QString buildDirPath = QDir::currentPath();
    setFilesList(buildDirPath);
    setFilesTree(buildDirPath);

    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &TreeFilesWidget::changeViewTypeSlot);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &TreeFilesWidget::listItemClicked);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &TreeFilesWidget::treeItemClicked);
}

TreeFilesWidget::~TreeFilesWidget()
{
    delete ui;
}

void TreeFilesWidget::setFilesList(QDir dir)
{
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name);
    for (const QFileInfo &fileInfo : fileList) {
        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
        item->setData(Qt::UserRole, fileInfo.filePath());
        ui->listWidget->addItem(item);

    }
}

void TreeFilesWidget::setFilesList(QString dirPath)
{
    QDir dir(dirPath);
    setFilesList(dir);
}

void TreeFilesWidget::setFilesTree(QDir dir)
{
    QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name);

    for (const QFileInfo &dirInfo : dirList) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, dirInfo.fileName());
        item->setData(0, Qt::UserRole, dirInfo.filePath());
        ui->treeWidget->addTopLevelItem(item);



        QDir subDir(dirInfo.filePath());
        QFileInfoList subFileList = subDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDir::Name);
        for (const QFileInfo &subFileInfo : subFileList) {
            QTreeWidgetItem *subItem = new QTreeWidgetItem(item);
            subItem->setText(0, subFileInfo.fileName());
            subItem->setData(0, Qt::UserRole, subFileInfo.filePath());
            item->addChild(subItem);

        }
    }

    for (const QFileInfo &fileInfo : fileList) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, fileInfo.fileName());
        item->setData(0, Qt::UserRole, fileInfo.filePath());
        ui->treeWidget->addTopLevelItem(item);

    }
}

void TreeFilesWidget::setFilesTree(QString dirPath)
{
    QDir dir(dirPath);
    setFilesTree(dir);
}

void TreeFilesWidget::changeViewTypeSlot()
{
    QString currentText = ui->comboBox->currentText();
    if(currentText =="Файлы и папки") {
        ui->stackedWidget->setCurrentIndex(0);
        currentTreeItem = nullptr;

    } else{
        ui->stackedWidget->setCurrentIndex(1);
        currentListItem = nullptr;

    }
}

void TreeFilesWidget::customContextMenuRequestedSlot(QPoint p){
    QMenu *menu = new QMenu(this);
    QAction *item1 = new QAction("Открыть", this);
    QAction *item2 = new QAction("Переименовать", this);
    QAction *item3 = new QAction("Удалить", this);

    connect(item1, &QAction::triggered, this, &TreeFilesWidget::item1Slot);
    connect(item2, &QAction::triggered, this, &TreeFilesWidget::item2Slot);
    connect(item3, &QAction::triggered, this, &TreeFilesWidget::item3Slot);

    menu->addAction(item1);
    menu->addAction(item2);
    menu->addAction(item3);
    menu->popup(this->mapToGlobal(p));
}

void TreeFilesWidget::item1Slot(){
    if (currentListItem) {
        QString filePath =currentListItem->data(Qt::UserRole).toString();
        qDebug() << "Open: " << filePath;
        emit openFileSignal(filePath);
    } else if (currentTreeItem) {
        QString filePath = currentTreeItem->data(0, Qt::UserRole).toString();
        qDebug() <<"Open: " << filePath;
        emit openFileSignal(filePath);
    }
}

void TreeFilesWidget::item2Slot(){
    if (currentListItem) {
        QString filePath =currentListItem->data(Qt::UserRole).toString();
        qDebug() <<"Rename: " << filePath;
        emit renameFileSignal(filePath);
    } else if (currentTreeItem) {
        QString filePath = currentTreeItem->data(0, Qt::UserRole).toString();
        qDebug()<< "Rename: " <<filePath;
        emit renameFileSignal(filePath);
    }
}

void TreeFilesWidget::item3Slot(){
    if (currentListItem) {
        QString filePath=currentListItem->data(Qt::UserRole).toString();
        qDebug() << "Delete: " <<filePath;
        delete currentListItem;
        currentListItem= nullptr;
        emit deleteFileSignal(filePath);
    } else if (currentTreeItem) {
        QString filePath = currentTreeItem->data(0, Qt::UserRole).toString();
        qDebug() << "Delete: " <<filePath;
        delete currentTreeItem;
        currentTreeItem= nullptr;
        emit deleteFileSignal(filePath);
    }
}

void TreeFilesWidget::listItemClicked(QListWidgetItem *item){
    currentListItem = item;
}

void TreeFilesWidget::treeItemClicked(QTreeWidgetItem *item, int column){
    currentTreeItem =item;
}
