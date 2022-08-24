//
// Created by bugae on 19.08.2022.
//

#ifndef GIS_GUI_MENUCONTROLLER_H
#define GIS_GUI_MENUCONTROLLER_H
#include <QAction>
#include <QMenuBar>
#include <QToolBar>


namespace gisUI {

    struct MapMenuActions{
        QAction *addLayerAction;
        QAction *createProjectAction;
        QAction *openProjectAction;
        QAction *saveProjectAction;
        QAction *saveAsProjectAction;

        MapMenuActions();
        ~MapMenuActions();
        MapMenuActions(const MapMenuActions *actions) = delete;
        MapMenuActions &operator=(const MapMenuActions *actions) = delete;
    };


    class MenuController:public QObject {
        Q_OBJECT
    private:
        MapMenuActions actions;

        void addMapActions(QWidget* widget);
    public:
        MenuController(QMenuBar* menuBar, QToolBar* toolBar);
    signals:
        void createProjectRequest();
        void openLayerRequest();
        void openProjectRequest();
        void saveProjectRequest();
        void saveAsProjectRequest();

    };
}

#endif //GIS_GUI_MENUCONTROLLER_H
