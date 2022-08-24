//
// Created by bugae on 19.08.2022.
//

#include "CentralWidget.h"


namespace gisUI {


    CentralWidget::CentralWidget(QWidget *parent):
    btns {[=] {
                mapBar = new QToolBar(this);
                return mapBar;}()}{

        toolLayout = new QBoxLayout(QBoxLayout::TopToBottom);
        toolLayout->setContentsMargins(0, 0, 0, 0);
        mapBar = new QToolBar(this);

        fillMapBar();
        toolLayout->addWidget(mapBar);
        mapWidget = new GISMapWidget(this);
        mapWidget->setMinimumSize(600,400);
        toolLayout->addWidget(mapWidget);
        setLayout(toolLayout);

        connect(actions.panAction, &QAction::triggered, mapWidget,&GISMapWidget::activatePan);
        connect(actions.zoomInAction, &QAction::triggered, mapWidget,&GISMapWidget::activateZoomIn);
        connect(actions.zoomOutAction, &QAction::triggered, mapWidget,&GISMapWidget::activateZoomOut);
        connect(actions.fullExtentAction, &QAction::triggered, mapWidget, &GISMapWidget::zoomToFullExtent);
    }

    CentralWidget::~CentralWidget() {
        if (toolLayout!= nullptr)
            delete toolLayout;
        if (mapBar!= nullptr)
            delete mapBar;
    }

    void CentralWidget::fillMapBar() {
        QWidget* spacer = new QWidget;
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        spacer->setVisible(true);

        mapBar->addAction(actions.panAction);
        mapBar->addAction(actions.zoomInAction);
        mapBar->addAction(actions.zoomOutAction);
        mapBar->addAction(actions.zoomToTrajAction);
        mapBar->addAction(actions.fullExtentAction);
        mapBar->addSeparator();
        mapBar->addAction(actions.pointsColor);
        mapBar->addWidget(spacer);
        mapBar->addWidget(btns.playBtn);
        mapBar->addWidget(btns.pauseBtn);
        mapBar->addWidget(btns.stopBtn);

    }

    void CentralWidget::setLayers(QList<QgsMapLayer *> layers) {
        mapWidget->updateLayers(layers);
    }

    GISMapWidget *CentralWidget::getMapWidget() const {
        return mapWidget;
    }
}