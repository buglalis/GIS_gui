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
        connect(this, &GISMapContoller::openLayerRequest, centralWidget, &CentralWidget::openLayerDialog);
        connect(manager, &GISMapLayerManager::sendLayers, centralWidget, &CentralWidget::setLayers);
        connect(centralWidget->getMapWidget(), &GISMapWidget::setNewVectorLayersRequest, manager, &GISMapLayerManager::setNewVectorLayers);
    }



    CentralWidget *GISMapContoller::getCentralWidget() const {
        return centralWidget;
    }

    void GISMapContoller::setCentralWidget(CentralWidget *centralWidget) {
        GISMapContoller::centralWidget = centralWidget;
    }

    void GISMapContoller::openLayer() {
        emit openLayerRequest();
    }
}