//
// Created by bugae on 19.08.2022.
//

#include "CentralWidget.h"


namespace gisUI {


    CentralWidget::CentralWidget(QWidget *parent) {
        toolLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        toolLayout->setContentsMargins(0, 0, 0, 0);
        mapBar = new QToolBar(this);
        actions = new MapActions;
        btns = new ModelingButtons(mapBar);
        fillMapBar();
        toolLayout->addWidget(mapBar);
        mapWidget = new GISMapWidget(this);
        mapWidget->setMinimumSize(600,400);
        toolLayout->addWidget(mapWidget);
        setLayout(toolLayout);
    }

    CentralWidget::~CentralWidget() {
        if (toolLayout!= nullptr)
            delete toolLayout;
        if (mapBar!= nullptr)
            delete mapBar;
        if (actions!= nullptr)
            delete actions;
        if (btns!= nullptr)
            delete btns;
    }

    void CentralWidget::fillMapBar() {
        QWidget* spacer = new QWidget;
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        spacer->setVisible(true);

        mapBar->addAction(actions->panAction);
        mapBar->addAction(actions->zoomInAction);
        mapBar->addAction(actions->zoomOutAction);
        mapBar->addAction(actions->zoomToTrajAction);
        mapBar->addAction(actions->fullExtentAction);
        mapBar->addSeparator();
        mapBar->addAction(actions->pointsColor);
        mapBar->addWidget(spacer);
        mapBar->addWidget(btns->playBtn);
        mapBar->addWidget(btns->pauseBtn);
        mapBar->addWidget(btns->stopBtn);

    }

    void CentralWidget::openLayerDialog() {
        mapWidget->addLayer();
    }

    void CentralWidget::setLayers(QList<QgsMapLayer *> layers) {
        mapWidget->updateLayers(layers);
    }

    GISMapWidget *CentralWidget::getMapWidget() const {
        return mapWidget;
    }

    void CentralWidget::setMapWidget(GISMapWidget *mapWidget) {
        CentralWidget::mapWidget = mapWidget;
    }


}