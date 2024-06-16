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
