//
// Created by bugae on 19.08.2022.
//

#include "MenuController.h"

namespace gisUI{
    MapMenuActions::MapMenuActions() {
        addLayerAction = new QAction(QIcon(":/icons/plus_mini2.png"), "Добавить слой");
        createProjectAction = new QAction(QIcon(":/icons/create.png"), "Создать проект");
        openProjectAction = new QAction(QIcon(":/icons/open.png"), "Открыть проект");
        saveProjectAction = new QAction(QIcon(":/icons/save.png"), "Сохранить проект");
        saveAsProjectAction = new QAction(QIcon(":/icons/saveAs.png"), "Сохранить как...");

        addLayerAction->setShortcut(Qt::CTRL + Qt::Key_V);
        createProjectAction->setShortcut(Qt::CTRL + Qt::Key_N);
        openProjectAction->setShortcut(Qt::CTRL + Qt::Key_O);
        saveProjectAction->setShortcut(Qt::CTRL + Qt::Key_S);
        saveAsProjectAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    }

    MapMenuActions::~MapMenuActions() {
        if (addLayerAction!= nullptr)
            delete addLayerAction;
        if (createProjectAction!= nullptr)
            delete createProjectAction;
        if (openProjectAction!= nullptr)
            delete openProjectAction;
        if (saveProjectAction!= nullptr)
            delete saveProjectAction;
        if (saveAsProjectAction!= nullptr)
            delete saveAsProjectAction;
    }

    MenuController::MenuController(QMenuBar *menuBar, QToolBar* toolBar) {
        QMenu* mapMenu = new QMenu("Карта");
        addMapActions(mapMenu);
        addMapActions(toolBar);
        menuBar->addMenu(mapMenu);

        connect(actions.createProjectAction, &QAction::triggered, this, &MenuController::createProjectRequest);
        connect(actions.addLayerAction, &QAction::triggered, this, &MenuController::openLayerRequest);
        connect(actions.openProjectAction, &QAction::triggered, this, &MenuController::openProjectRequest);
        connect(actions.saveProjectAction, &QAction::triggered, this, &MenuController::saveProjectRequest);
        connect(actions.saveAsProjectAction, &QAction::triggered, this, &MenuController::saveAsProjectRequest);
    }

    void MenuController::addMapActions(QWidget *widget) {
       widget->addAction(actions.addLayerAction);
       QAction* separatorAction = new QAction;
       separatorAction->setSeparator(true);
       widget->addAction(separatorAction);
       widget->addAction(actions.createProjectAction);
       widget->addAction(actions.openProjectAction);
       widget->addAction(actions.saveProjectAction);
       widget->addAction(actions.saveAsProjectAction);
       widget->addAction(separatorAction);
    }
}