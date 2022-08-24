//
// Created by bugae on 24.08.2022.
//

#ifndef GIS_GUI_DATABASECONTROLLER_H
#define GIS_GUI_DATABASECONTROLLER_H


#include <QObject>
#include "DatabaseWorker.h"
#include <QMenuBar>
#include "DatabaseBrowser.h"

namespace gisUI {
    class DatabaseController : public QObject {
    Q_OBJECT
    private:
        DatabaseWorker* worker = nullptr;
        DatabaseBrowser* browser = nullptr;

        QMenu* dbMenu = nullptr;
    public:
        QMenu *getDbMenu() const;

        void setDbMenu(QMenu *dbMenu);

    private:
        QAction* editDb = nullptr;
    public:
        DatabaseController();


    };
}

#endif //GIS_GUI_DATABASECONTROLLER_H
