//
// Created by bugae on 17.08.2022.
//

// You may need to build the project (run Qt uic code generator) to get "ui_GISMainWindow.h" resolved

#include "gismainwindow.h"
#include <QDebug>
#include <qgslayertreemapcanvasbridge.h>
#include "widgets/map/GISMapWidget.h"
namespace gisUI {

    GISMainWindow::GISMainWindow(QWidget *parent) :
            QMainWindow(parent){
        toolBar = new QToolBar;
        addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolBar);
        menuController = new MenuController(this->menuBar(),toolBar);
        mapController = new GISMapContoller(this);
        setCentralWidget(mapController->getCentralWidget());
        connect(menuController, &MenuController::openLayerRequest, mapController, &GISMapContoller::openLayer);

    }
}


