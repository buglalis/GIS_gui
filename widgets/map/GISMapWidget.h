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
        std::unique_ptr<QgsMapToolIdentifyFeature> identifyFeature;
        std::unique_ptr<QgsMapToolZoom> zoomInTool;
        std::unique_ptr<QgsMapToolZoom> zoomOutTool;
        std::unique_ptr<QgsMapToolPan> panTool;

        std::unique_ptr<QgsMapToolEmitPoint> emitPointTool;
        std::unique_ptr<QgsMapToolIdentify> identifyTool;

        QCursor zoomInCursor;
        QCursor zoomOutCursor;
        QCursor identifyCursor;

        MapTools(QgsMapCanvas* canvas);
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
        GISMapWidget(QWidget* parent = nullptr);
        ~GISMapWidget();

        void updateLayers(QList<QgsMapLayer*> layers);
    public slots:
        void addLayer();
        void setupExtent(QgsRectangle rect);
        void setDestCoords(QgsCoordinateReferenceSystem coords);
    signals:
        void layerAdded();
        void setNewVectorLayersRequest(QList<QgsVectorLayer*> layers);
    private:

        MapTools tools;



//        void setNewVectorLayers(std::vector<std::unique_ptr<QgsVectorLayer>> layers);
    };
}

#endif //GIS_GUI_GISMAPWIDGET_H
