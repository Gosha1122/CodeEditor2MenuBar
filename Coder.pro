QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    colorscheme.cpp \
    configreader.cpp \
    dialog.cpp \
    highlighter.cpp \
    highlighterJson.cpp \
    highlighterQss.cpp \
    main.cpp \
    mainwindow.cpp \
    stylehelper.cpp

HEADERS += \
    codeeditor.h \
    colorscheme.h \
    configreader.h \
    dialog.h \
    highlighter.h \
    highlighterJson.h \
    highlighterQss.h \
    mainwindow.h \
    stylehelper.h

FORMS += \
    dialog.ui \
    mainwindow.ui

DISTFILES += \
    default.conf

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourses.qrc




