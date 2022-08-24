//
// Created by bugae on 24.08.2022.
//

#include "DatabaseController.h"

namespace gisUI {
    gisUI::DatabaseController::DatabaseController() {
        dbMenu = new QMenu("Данные");
        editDb = new QAction(QIcon(":/icons/archive.svg"), "Обзор/Редактирование ЛТХ");
        dbMenu->addAction(editDb);
        worker = new DatabaseWorker;
        browser = new DatabaseBrowser;
        connect(editDb, &QAction::triggered, browser, &DatabaseBrowser::show);

    }

    QMenu *DatabaseController::getDbMenu() const {
        return dbMenu;
    }

    void DatabaseController::setDbMenu(QMenu *dbMenu) {
        DatabaseController::dbMenu = dbMenu;
    }
}