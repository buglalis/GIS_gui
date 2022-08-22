#include <QApplication>
#include <QDir>

#include <stdlib.h>
#include <iostream>
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgscolorschemeregistry.h>
#include "gismainwindow.h"

class RecentScheme : public QgsRecentColorScheme
{
public:
    RecentScheme() : QgsRecentColorScheme() {}
    ~RecentScheme() {}
    virtual bool isEditable()const {return true;}
};

int main(int argc, char *argv[]) {

    QgsApplication a(argc, argv,true);
    a.setMaxThreads(-1);
    a.colorSchemeRegistry()->addColorScheme(new RecentScheme());
    QFile file(QString("%1/Aqua.qss").arg(QDir::currentPath()));
    if (file.open(QIODevice::ReadOnly)){
        QString style = QLatin1String(file.readAll());
        qApp->setStyleSheet(style);
    }

    gisUI::GISMainWindow window;
    window.show();
    return a.exec();

}
