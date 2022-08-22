//
// Created by bugae on 19.08.2022.
//

#ifndef GIS_GUI_GISMAPCONTOLLER_H
#define GIS_GUI_GISMAPCONTOLLER_H

#include <qgslayertreemapcanvasbridge.h>
#include "GISMapLayerManager.h"
#include "CentralWidget.h"
namespace gisUI {

    class GISMapContoller: public QObject{
    Q_OBJECT
    public:
        GISMapContoller(QWidget* parent = nullptr);

    private:
        CentralWidget* centralWidget;
        GISMapLayerManager *manager;
        QgsLayerTreeMapCanvasBridge *bridge;
    public:
        CentralWidget *getCentralWidget() const;

        void setCentralWidget(CentralWidget *centralWidget);

    signals:
        void openLayerRequest();
    public slots:
        void openLayer();
    };
}

#endif //GIS_GUI_GISMAPCONTOLLER_H
