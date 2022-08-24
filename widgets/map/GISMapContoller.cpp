//
// Created by bugae on 19.08.2022.
//

#include "GISMapContoller.h"
#include <QObject>
namespace gisUI {

    GISMapContoller::GISMapContoller(QWidget* parent) {
        centralWidget = new CentralWidget;
        manager = new GISMapLayerManager;
        bridge = new QgsLayerTreeMapCanvasBridge(manager->getRoot(), centralWidget->getMapWidget());
        connect(this, &GISMapContoller::openLayerFileDialogRequest, centralWidget->getMapWidget(), &GISMapWidget::addLayer);
        connect(this, &GISMapContoller::openProjectFileDialogRequest, centralWidget->getMapWidget(), &GISMapWidget::openProject);
        connect(this, &GISMapContoller::saveProjectFileDialogRequest, centralWidget->getMapWidget(), &GISMapWidget::saveProject);
        connect(this, &GISMapContoller::saveAsProjectFileDialogRequest,centralWidget->getMapWidget(), &GISMapWidget::saveAsProject);
        connect(this, &GISMapContoller::createProjectMessageBoxRequest,centralWidget->getMapWidget(), &GISMapWidget::createProject);


        connect(manager, &GISMapLayerManager::sendLayers, centralWidget, &CentralWidget::setLayers);
        connect(centralWidget->getMapWidget(), &GISMapWidget::setNewVectorLayersRequest, manager, &GISMapLayerManager::setNewVectorLayers);
        connect(centralWidget->getMapWidget(), &GISMapWidget::readLayersFromProjRequest, manager, &GISMapLayerManager::readLayersFromProj);
        connect(centralWidget->getMapWidget(), &GISMapWidget::clearLayersRequest, manager, &GISMapLayerManager::clearLayers);
    }



    CentralWidget *GISMapContoller::getCentralWidget() const {
        return centralWidget;
    }

    void GISMapContoller::setCentralWidget(CentralWidget *centralWidget) {
        GISMapContoller::centralWidget = centralWidget;
    }

}