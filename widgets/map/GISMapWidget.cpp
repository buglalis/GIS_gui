//
// Created by bugae on 17.08.2022.
//

#include <QFileDialog>
#include <utility>
#include <qgssymbol.h>
#include <qgssinglesymbolrenderer.h>
#include <QDebug>
#include <QMessageBox>
#include "GISMapWidget.h"

namespace gisUI {

    MapTools::MapTools(QgsMapCanvas *canvas) {
        panTool       = new QgsMapToolPan(canvas);
        zoomInCursor  = new QCursor(QPixmap(":/icons/zoomIn.png").scaled(20, 20, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        zoomOutCursor = new QCursor(QPixmap(":/icons/zoomOut.png").scaled(20, 20, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        zoomInTool    = new QgsMapToolZoom(canvas, false);
        zoomOutTool   = new QgsMapToolZoom(canvas, true);
        zoomInTool->setCursor(*zoomInCursor);
        zoomOutTool->setCursor(*zoomOutCursor);
        //инструмент выделения точек
        identifyCursor = new QCursor(QPixmap(":/icons/position.png").scaled(20, 20, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        identifyFeature   = new QgsMapToolIdentifyFeature(canvas);
        identifyFeature->setCursor(*identifyCursor);
        identifyTool      = new QgsMapToolIdentify(canvas);
        emitPointTool     = new QgsMapToolEmitPoint (canvas);
    }

    MapTools::~MapTools() {
        if (panTool!=nullptr)
            delete panTool;
        if (zoomInTool!=nullptr)
            delete zoomInTool;
        if (zoomOutTool!=nullptr)
            delete zoomOutTool;
        if (identifyTool!=nullptr)
            delete identifyTool;
        if (zoomInCursor!=nullptr)
            delete zoomInCursor;
        if (zoomOutCursor!=nullptr)
            delete zoomOutCursor;
        if (identifyCursor!=nullptr)
            delete identifyCursor;

    }

    QList<QgsVectorLayer *> LayerHandler::handleLayerBase(const QString &layerPath, const QString& layerName) const {
        QgsVectorLayer::LayerOptions options;
        options.loadDefaultStyle = false;

        QString onlyName = layerName.mid(layerName.lastIndexOf("/") + 1);
        if (onlyName == "points" ||
            onlyName == "controlPoints" ||
            onlyName == "controlLines" ||
            onlyName == "trajectories") {
//           ToDo: createWarningWindow("Ошибка!", "Нельзя добавить слой с названием points/controlPoints/controlLines/trajectories");
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
        setMapUpdateInterval(500); //ToDO::check possible values

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

    void GISMapWidget::openProject() {
        switch(resetMessageBox()) {
            case QMessageBox::Save:
                saveAsProject();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
        }
        QFileDialog dialog(this, QString("Открыть проект").toLocal8Bit(), "", QString("*.qgs").toLocal8Bit());
        dialog.setOption(QFileDialog::DontUseNativeDialog);
        if (dialog.exec() == QDialog::Accepted)
        {
//            emit projectClosed(); ToDo:: create signal when working with traj
            QString path = dialog.selectedFiles().at(0);
            emit readLayersFromProjRequest(path, mapSettings().destinationCrs());
        }
    }

    void GISMapWidget::saveProject() {
        if (QgsProject::instance()->fileInfo().exists())
        {
//            emit auxLayersDeletedRequest();//ToDO:: add func
            QgsProject::instance()->write();
//            emit auxLayersAddedRequest(); //ToDO:: add func
        }
        else
            saveAsProject();
    }

    void GISMapWidget::saveAsProject() {
        QString dir = "";
        QString filename = "";
        if (QgsProject::instance()->fileInfo().exists())
        {
            dir = QgsProject::instance()->absolutePath();
            filename = QgsProject::instance()->fileName();
        }
        QFileDialog dialog(this, QString("Сохранить проект").toLocal8Bit(), dir, QString("*.qgs").toLocal8Bit());
        dialog.setOption(QFileDialog::DontUseNativeDialog);
        if (filename != "")
            dialog.selectFile(filename);
        dialog.setDefaultSuffix("qgs");
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        if (dialog.exec() == QDialog::Accepted)
        {
            QString str = dialog.selectedFiles().at(0);
            if (!str.endsWith(".qgs"))
                str += ".qgs";
            QgsProject::instance()->setFileName(str);
//            emit auxLayersDeletedRequest();//ToDO:: add func
            QgsProject::instance()->write();
//            emit auxLayersAddedRequest(); //ToDO:: add func
        }
    }

    void GISMapWidget::createProject() {
        switch(resetMessageBox()) {
            case QMessageBox::Save:
                saveAsProject();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
        }
        //ToDO:: уведомление о том, что текущий сценарий будет удален
//        emit projectClosed();
//        userAddedLayers.clear();
        QgsProject::instance()->clear();
        refresh();
    }

    int GISMapWidget::resetMessageBox() {
        QMessageBox* box = new QMessageBox(this);
        box->setWindowTitle("Сохранить изменения?");
        box->setText("Изменения в текущем проекте не будут сохранены");
        box->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        box->setButtonText(QMessageBox::Save, "Сохранить");
        box->setButtonText(QMessageBox::Discard, "Не сохранить");
        box->setButtonText(QMessageBox::Cancel, "Отменить");
        return box->exec();
    }

    void GISMapWidget::activatePan() {
        if (tools.activated != MapTools::Tools::PAN) {
            setMapTool(tools.panTool);
            tools.activated = MapTools::Tools::PAN;
        }
        else {
            unsetMapTool(tools.panTool);
            tools.activated = MapTools::Tools::NONE;
        }
    }

    void GISMapWidget::activateZoomIn() {
        if (tools.activated != MapTools::Tools::ZOOM_IN) {
            setMapTool(tools.zoomInTool);
            tools.activated = MapTools::Tools::ZOOM_IN;
        }
        else {
            unsetMapTool(tools.zoomInTool);
            tools.activated = MapTools::Tools::NONE;
        }
    }

    void GISMapWidget::activateZoomOut() {
        if (tools.activated != MapTools::Tools::ZOOM_OUT) {
            setMapTool(tools.zoomOutTool);
            tools.activated = MapTools::Tools::ZOOM_OUT;
        }
        else {
            unsetMapTool(tools.zoomOutTool);
            tools.activated = MapTools::Tools::NONE;
        }
    }
}