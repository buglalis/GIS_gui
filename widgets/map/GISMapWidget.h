//
// Created by bugae on 17.08.2022.
//

#ifndef GIS_GUI_GISMAPWIDGET_H
#define GIS_GUI_GISMAPWIDGET_H

#include <qgsmapcanvas.h>
#include <qgsmaptoolzoom.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolidentifyfeature.h>
#include <memory>
#include <qgsmaptoolemitpoint.h>
#include <qgsvectorlayer.h>

namespace gisUI {


    struct MapTools{
        enum class Tools{PAN,ZOOM_IN, ZOOM_OUT, IDENTIFY, EMIT_POINT, NONE};
       QgsMapToolIdentifyFeature* identifyFeature;
       QgsMapToolZoom* zoomInTool;
       QgsMapToolZoom* zoomOutTool;
       QgsMapToolPan* panTool;

       QgsMapToolEmitPoint* emitPointTool;
       QgsMapToolIdentify* identifyTool;

        QCursor* zoomInCursor;
        QCursor* zoomOutCursor;
        QCursor* identifyCursor;

        Tools activated = Tools::NONE;
        MapTools(QgsMapCanvas* canvas);
        ~MapTools();
        MapTools(const MapTools& other) = delete;
        MapTools& operator=(const MapTools& other) = delete;
    };

    class LayerHandler {
    public:
        LayerHandler() = default;
        virtual ~LayerHandler() = default;
        virtual QList<QgsVectorLayer *> handleLayer(const QString& layerPath) const = 0;

    protected:
        QList<QgsVectorLayer *> handleLayerBase(const QString &layerPath, const QString& layerName) const;
    };


    class ShpLayerHandler final: public LayerHandler {
        public:
        QList<QgsVectorLayer *> handleLayer(const QString& layerPath)  const override;
    };

    class S57LayerHandler final: public LayerHandler {
    public:
        QList<QgsVectorLayer *> handleLayer(const QString& layerPath)  const override;
    };

    class SxfLayerHandler final: public LayerHandler {
    public:
        QList<QgsVectorLayer *> handleLayer(const QString& layerPath)  const override;
    };



    class GISMapWidget: public QgsMapCanvas {
        Q_OBJECT
    public:
        explicit GISMapWidget(QWidget* parent = nullptr);
        ~GISMapWidget();

        void updateLayers(QList<QgsMapLayer*> layers);
    public slots:
        void addLayer();
        void openProject();
        void saveProject();
        void saveAsProject();
        void createProject();

        void activatePan();
        void activateZoomIn();
        void activateZoomOut();

        void setupExtent(QgsRectangle rect);
        void setDestCoords(QgsCoordinateReferenceSystem coords);
    signals:
        void layerAdded();
        void setNewVectorLayersRequest(QList<QgsVectorLayer*> layers);
        void readLayersFromProjRequest(const QString& path, const QgsCoordinateReferenceSystem destCoord);
        void clearLayersRequest();
    private:
        MapTools tools;
        int resetMessageBox();

    };
}

#endif //GIS_GUI_GISMAPWIDGET_H
