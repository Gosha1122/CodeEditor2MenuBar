#include "stylehelper.h"
#include <QFontDatabase>
#include <QDebug>
void StyleHelper::setFonts()
{
    QString fontUrl[] = { QStringLiteral(":/fonts/consolas.ttf"),
                          QStringLiteral(":/fonts/Roboto-Bold.ttf"),
                          QStringLiteral(":/fonts/Roboto-Italic.ttf"),
                          QStringLiteral(":/fonts/Roboto-Regular.ttf")
                        };
    for(auto& url: fontUrl){
        int id = QFontDatabase::addApplicationFont(url);
        qDebug() <<  QFontDatabase::applicationFontFamilies(id);
    }
}

QString StyleHelper::getMainStyleLight()
{
    return "QWidget{"
           "font-family:Roboto;"
           "font-size:12px;"
           "}";
}

QString StyleHelper::StyleHelperTabWidget(int status)
{
    QString Style;
    if(status == 0){
        Style = "QTabBar::tab{"
                "border: 4px solid #cccccc;"
                "border-top-left-radius: 7px;"
                "border-top-right-radius: 7px;"
                "background-color: #cccccc;"
                "padding-left: 10px;"
                "margin-left: 3px;"
                "}"
                "QTabBar::tab:hover{"
                "background-color: #eeeedd;"
                "border-color: #eeeedd;"
                "}"
                "QTabBar::tab:selected{"
                "background-color: #ffffff;"
                "border-color: #ffffff;"
                "}"
                "QTabWidget::pane{"
                "}"
                "QTabBar::close-button{"
                "image: url(:/images/close-btn3.png);"
                "}"
                "QTabBar::close-button:hover{"
                "image: url(:/images/close-btn.png)"
                "}";
    }else if(status == 1){
        Style = "QTabBar::tab{"
                "border: 4px solid #cccccc;"
                "border-top-left-radius: 7px;"
                "border-top-right-radius: 7px;"
                "background-color: #cccccc;"
                "padding-left: 10px;"
                "margin-left: 3px;"
                "}"
                "QTabBar::tab:hover{"
                "background-color: #eeeedd;"
                "border-color: #eeeedd;"
                "}"
                "QTabBar::tab:selected{"
                "background-color: #ffffff;"
                "border-color: #ffffff;"
                "}"
                "QTabWidget::pane{"
                "}"
                "QTabBar::close-button{"
                "image: none;"
                "}";
    }
    return Style;
}
