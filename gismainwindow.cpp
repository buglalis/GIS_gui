//
// Created by bugae on 17.08.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GISMainWindow.h" resolved

#include "gismainwindow.h"
#include <QDebug>

namespace gisUI {

    GISMainWindow::GISMainWindow(QWidget *parent) :
            QMainWindow(parent){
        toolBar = new QToolBar;
        addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolBar);
        menuController = new MenuController(this->menuBar(),toolBar);
        mapController = new GISMapContoller(this);
        setCentralWidget(mapController->getCentralWidget());
        connect(menuController, &MenuController::openLayerRequest, mapController, &GISMapContoller::openLayerFileDialogRequest);
        connect(menuController, &MenuController::openProjectRequest, mapController, &GISMapContoller::openProjectFileDialogRequest);
        connect(menuController, &MenuController::saveProjectRequest, mapController, &GISMapContoller::saveProjectFileDialogRequest);
        connect(menuController, &MenuController::saveAsProjectRequest, mapController, &GISMapContoller::saveAsProjectFileDialogRequest);
        connect(menuController, &MenuController::createProjectRequest, mapController, &GISMapContoller::createProjectMessageBoxRequest);

        dbController = new DatabaseController;
        this->menuBar()->addMenu(dbController->getDbMenu());

    }
}


