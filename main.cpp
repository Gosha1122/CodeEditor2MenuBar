#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
//g
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(translator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(&translator);
#else
    if(translator.load("qt_" + QLocale::system().name(), QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        a.installTranslator(&translator);
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
