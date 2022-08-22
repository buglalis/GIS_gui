//
// Created by bugae on 17.08.2022.
//

#include <QFileDialog>
#include <utility>
#include <qgssymbol.h>
#include <qgssinglesymbolrenderer.h>
#include <QDebug>
#include "GISMapWidget.h"

namespace gisUI {

    MapTools::MapTools(QgsMapCanvas *canvas) {
        panTool       = std::make_unique<QgsMapToolPan>(canvas);
        zoomInCursor  = QCursor(QPixmap(":/icons/zoomIn.png").scaled(20, 20, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        zoomOutCursor = QCursor(QPixmap(":/icons/zoomOut.png").scaled(20, 20, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        zoomInTool    = std::make_unique<QgsMapToolZoom>(canvas, false);
        zoomOutTool   = std::make_unique<QgsMapToolZoom>(canvas, true);
        zoomInTool->setCursor(zoomInCursor);
        zoomOutTool->setCursor(zoomOutCursor);
        //инструмент выделения точек
        identifyCursor = QCursor(QPixmap(":/icons/position.png").scaled(20, 20, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        identifyFeature   = std::make_unique<QgsMapToolIdentifyFeature>(canvas);
        identifyFeature->setCursor(identifyCursor);
        identifyTool      = std::make_unique<QgsMapToolIdentify>(canvas);
        emitPointTool     = std::make_unique<QgsMapToolEmitPoint>(canvas);
    }

    QList<QgsVectorLayer *> LayerHandler::handleLayerBase(const QString &layerPath, const QString& layerName) const {
        QgsVectorLayer::LayerOptions options;
        options.loadDefaultStyle = false;

        QString onlyName = layerName.mid(layerName.lastIndexOf("/") + 1);
        if (onlyName == "points" ||
            onlyName == "controlPoints" ||
            onlyName == "controlLines" ||
            onlyName == "trajectories") {
//            createWarningWindow("Ошибка!", "Нельзя добавить слой с названием points/controlPoints/controlLines/trajectories");
            return {};
        }

        QgsVectorLayer* layer = new QgsVectorLayer(layerPath, layerName, "ogr", options);
        if ( !layer->isValid() )
            return {};
        QList<QgsVectorLayer*> layers;
        layers.push_back(layer);
        return layers;
    }

    QList<QgsVectorLayer *> ShpLayerHandler::handleLayer(const QString &layerPath) const {
        QString layerName = layerPath;
        layerName.remove(".shp");
        return LayerHandler::handleLayerBase(layerPath,layerName);
    }

    QList<QgsVectorLayer *> S57LayerHandler::handleLayer(const QString &layerPath) const {
        QString layerName = layerPath;
        layerName.remove(".000");
        return LayerHandler::handleLayerBase(layerPath,layerName);
    }

    QList<QgsVectorLayer *> SxfLayerHandler::handleLayer(const QString &layerPath) const {
        QString layerName = layerPath;
        layerName.remove(".sxf");
        return LayerHandler::handleLayerBase(layerPath,layerName);
    }

    GISMapWidget::GISMapWidget(QWidget *parent) : QgsMapCanvas(parent), tools(this) {
        enableAntiAliasing(true);
        setCanvasColor(QColor(255, 255, 255));
        setVisible(true);
        setMapSettingsFlags(mapSettings().flags() | QgsMapSettings::RenderPreviewJob);
        setParallelRenderingEnabled(true);
        setCachingEnabled(true);
        setPreviewJobsEnabled(true);
        setMapUpdateInterval(100); //ToDO::check possible values

    }

    GISMapWidget::~GISMapWidget() {
    }



    void GISMapWidget::addLayer() {
        QFileDialog dialog(this, QString("Добавить слой").toLocal8Bit());
        dialog.setOption(QFileDialog::DontUseNativeDialog);
        dialog.setNameFilter("*.shp *.000 *.sxf");

        if (dialog.exec() == QDialog::Accepted) {
            QString layerPath =dialog.selectedFiles()[0];
            if (!QFile::exists(layerPath))
                return;
            std::unique_ptr<LayerHandler> layerHandler;
            if (layerPath.contains(".shp"))
                layerHandler = std::make_unique<ShpLayerHandler>();
            else if (layerPath.contains(".000"))
                layerHandler = std::make_unique<S57LayerHandler>();
            else if (layerPath.contains(".sxf"))
                layerHandler = std::make_unique<SxfLayerHandler>();
            else return; //ToDO:: make a message about non-supported types
            setNewVectorLayersRequest(layerHandler->handleLayer(layerPath));
        }
    }

    void GISMapWidget::updateLayers(QList<QgsMapLayer *> layers) {
        QgsProject::instance()->addMapLayers(layers);
        setLayers(layers);
        refresh();
        emit layerAdded();
    }

    void GISMapWidget::setupExtent(QgsRectangle rect) {
        setExtent(rect);
    }

    void GISMapWidget::setDestCoords(QgsCoordinateReferenceSystem coords) {
        setDestinationCrs(coords);
    }
}