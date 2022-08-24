//
// Created by bugae on 19.08.2022.
//

#ifndef GIS_GUI_CENTRALWIDGET_H
#define GIS_GUI_CENTRALWIDGET_H


#include <QWidget>
#include <QBoxLayout>
#include "../tools/MapActions.h"
#include "../tools/CustomControlButton.h"
#include "GISMapWidget.h"

namespace gisUI {
    class CentralWidget : public QWidget {
    Q_OBJECT
    public:
        explicit CentralWidget(QWidget *parent = nullptr);

        ~CentralWidget();

        CentralWidget(const CentralWidget *widget) = delete;

        CentralWidget &operator=(const CentralWidget *widget) = delete;

    private:
        QBoxLayout *toolLayout;
        QToolBar* mapBar;
        MapActions actions;
        ModelingButtons btns;

        GISMapWidget* mapWidget;
    public:
        GISMapWidget *getMapWidget() const;
        void setMapWidget(GISMapWidget *mapWidget);
    private:
        void fillMapBar();

    public slots:
        void setLayers(QList<QgsMapLayer*> layers);
    };
}

#endif //GIS_GUI_CENTRALWIDGET_H
