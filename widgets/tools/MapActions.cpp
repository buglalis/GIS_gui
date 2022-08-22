//
// Created by bugae on 19.08.2022.
//

#include "MapActions.h"

namespace gisUI {
    MapActions::MapActions() {
        pointsColor = new QAction(QIcon(":/icons/changeColor.png"), QString("Изменить цвет точек"));
        zoomInAction = new QAction(QIcon(":/icons/zoomIn.png"), QString("Приблизить").toLocal8Bit());
        zoomOutAction = new QAction(QIcon(":/icons/zoomOut.png"), QString("Отдалить").toLocal8Bit());
        panAction = new QAction(QIcon(":/icons/grab.png"), QString("Прокрутка карты").toLocal8Bit());
        fullExtentAction = new QAction(QIcon(":/icons/full.png"), QString("Полный охват").toLocal8Bit());
        zoomToTrajAction = new QAction(QIcon(":/icons/zoomToTraj.png"),
                                       QString("Увеличить до траекторий").toLocal8Bit());
    }

    MapActions::~MapActions() {
        if (pointsColor != nullptr)
            delete pointsColor;
        if (zoomInAction != nullptr)
            delete zoomInAction;
        if (zoomOutAction != nullptr)
            delete zoomOutAction;
        if (panAction != nullptr)
            delete panAction;
        if (fullExtentAction != nullptr)
            delete fullExtentAction;
        if (zoomToTrajAction != nullptr)
            delete zoomToTrajAction;
    }

    ModelingButtons::ModelingButtons(QToolBar* toolBar) {
        playBtn = new CustomControlButton(toolBar);
        playBtn->setMinimumWidth(50);
        playBtn->setIcon(QIcon(":/icons/play.png"));

        pauseBtn = new CustomControlButton(toolBar);
        pauseBtn->setMinimumWidth(50);
        pauseBtn->setIcon(QIcon(":/icons/pause.png"));

        stopBtn = new CustomControlButton(toolBar);
        stopBtn->setMinimumWidth(50);
        stopBtn->setIcon(QIcon(":/icons/stop.png"));
    }

    ModelingButtons::~ModelingButtons() {
        if (playBtn!= nullptr)
            delete playBtn;
        if (stopBtn!= nullptr)
            delete stopBtn;
        if (pauseBtn!= nullptr)
            delete pauseBtn;
    }
}