#ifndef TREEFILESWIDGET_H
#define TREEFILESWIDGET_H

#include <QFrame>
#include <QDir>
#include <QListWidgetItem>
#include <QTreeWidgetItem>


namespace Ui {
class TreeFilesWidget;
}

class TreeFilesWidget : public QFrame
{
    Q_OBJECT

public:
    explicit TreeFilesWidget(QWidget *parent = nullptr);
    ~TreeFilesWidget();

    void setFilesList(QDir dir);
    void setFilesList(QString dirPath);
    void setFilesTree(QDir dir);
    void setFilesTree(QString dirPath);

private slots:
    void changeViewTypeSlot();
    void customContextMenuRequestedSlot(QPoint p);
    void item1Slot();
    void item2Slot();
    void item3Slot();
    void listItemClicked(QListWidgetItem *item);
    void treeItemClicked(QTreeWidgetItem *item, int column);

signals:
    void openFileSignal(const QString &filePath);
    void renameFileSignal(const QString &filePath);
    void deleteFileSignal(const QString &filePath);

private:
    Ui::TreeFilesWidget *ui;
    QListWidgetItem *currentListItem;   // Указатель на текущий элемент списка
    QTreeWidgetItem *currentTreeItem;   // Указатель на текущий элемент дерева

};

#endif // TREEFILESWIDGET_H
