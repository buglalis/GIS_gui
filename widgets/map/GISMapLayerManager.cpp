//
// Created by bugae on 19.08.2022.
//

#include <QDir>
#include <qgsfields.h>
#include <qgsvectorfilewriter.h>
#include <qgssinglesymbolrenderer.h>
#include "GISMapLayerManager.h"

namespace gisUI {
    Coordinates::Coordinates() {
        wgs84.createFromProj("+proj=longlat +datum=WGS84 +no_defs");
        mainCrs.createFromProj("+proj=longlat +datum=WGS84 +no_defs");
        currentCrs.createFromProj("+proj=longlat +datum=WGS84 +no_defs");
    }

    CoordinateTransform::CoordinateTransform() {

    }

    GISMapLayerManager::GISMapLayerManager() {
        createAuxiliaryLayers();
        root = QgsProject::instance()->layerTreeRoot();
    }

    void GISMapLayerManager::createAuxiliaryLayers() {
        QDir currentDir = QDir::currentPath();
        currentDir.mkdir("auxiliaryLayers");
        currentDir.mkdir("auxiliaryLayers/pointsFolder");
        currentDir.mkdir("auxiliaryLayers/controlPointsFolder");
        currentDir.mkdir("auxiliaryLayers/controlLinesFolder");
        QgsFields f;
        //слой контрольных точек
        f.append(QgsField("numTr", QVariant::Int));
        f.append(QgsField("numPoint", QVariant::Int));
        QgsVectorFileWriter shp2(QDir::currentPath() + "/auxiliaryLayers/controlPointsFolder/controlPoints.shp", "System", f, QgsWkbTypes::Point, QgsCoordinateReferenceSystem(), "ESRI Shapefile");
        f.clear();
        //слой контрольных линий
        f.append(QgsField("numTr", QVariant::Int));
        QgsVectorFileWriter shp3(QDir::currentPath() + "/auxiliaryLayers/controlLinesFolder/controlLines.shp", "System", f, QgsWkbTypes::LineString, QgsCoordinateReferenceSystem(), "ESRI Shapefile");
        f.clear();
        //слой точек
        f.append(QgsField("numTr", QVariant::Int));
        f.append(QgsField("isLast", QVariant::Bool));
        f.append(QgsField("objType", QVariant::String));
        f.append(QgsField("angle", QVariant::Double));
        f.append(QgsField("fullName", QVariant::String));
        QgsVectorFileWriter shp4(QDir::currentPath() + "/auxiliaryLayers/pointsFolder/points.shp", "System", f, QgsWkbTypes::Point, QgsCoordinateReferenceSystem(), "ESRI Shapefile");
        f.clear();
    }

    void GISMapLayerManager::setNewVectorLayers(QList<QgsVectorLayer*> layers) {
        setMainCrs(layers.at(0)->crs());
        QList<QgsMapLayer*> currentLayers;
        for (unsigned i = 0; i < layers.size(); i++)
        {
            layers.at(i)->setRenderer(new QgsSingleSymbolRenderer(QgsSymbol::defaultSymbol(layers.at(i)->geometryType())));
            currentLayers.push_back(layers.at(i));
            QFileInfo name (layers.at(i)->name());
            currentLayers.back()->setName(name.baseName());
        }
        if (layers.at(0)->crs() != coords.mainCrs)
            sendExtent(getLayerExtentInMainCrs(layers.at(0)->crs(), layers.at(0)->extent()));
        else
            sendExtent(layers.at(0)->extent());
        sendLayers(userAddedLayers+currentLayers);
        userAddedLayers+=currentLayers;
    }

    void GISMapLayerManager::setMainCrs(const QgsCoordinateReferenceSystem &crs) {
        if (userAddedLayers.size() == 0) {
            coords.mainCrs = crs;
            sendDestCoords(coords.mainCrs);
            coords.currentCrs = coords.mainCrs;
            transforms.currentCrsToWgs84 = QgsCoordinateTransform(coords.currentCrs, coords.wgs84,
                                                                  QgsCoordinateTransformContext());
            transforms.wgs84ToMainCrs = QgsCoordinateTransform(coords.wgs84, coords.mainCrs,
                                                               QgsCoordinateTransformContext());
            transforms.mainCrsToWgs84 = QgsCoordinateTransform(coords.mainCrs, coords.wgs84,
                                                               QgsCoordinateTransformContext());
        }
    }

    QgsRectangle
    GISMapLayerManager::getLayerExtentInMainCrs(const QgsCoordinateReferenceSystem &crs, const QgsRectangle &oldExtent) {
        return getLayerExtentInCrs(crs,coords.mainCrs, oldExtent);
    }

    QgsRectangle
    GISMapLayerManager::getLayerExtentInWgs84(const QgsCoordinateReferenceSystem &crs, const QgsRectangle &oldExtent) {
        return getLayerExtentInCrs(crs,coords.wgs84, oldExtent);
    }

    QgsRectangle
    GISMapLayerManager::getLayerExtentInCurrentCrs(const QgsCoordinateReferenceSystem &crs, const QgsRectangle &oldExtent) {
        return getLayerExtentInCrs(crs,coords.currentCrs, oldExtent);
    }

    QgsRectangle GISMapLayerManager::getLayerExtentInCrs(const QgsCoordinateReferenceSystem &crs,
                                                   const QgsCoordinateReferenceSystem &dstCrs,
                                                   const QgsRectangle &oldExtent) {
        QgsCoordinateTransform transformForExtentent(crs, dstCrs, QgsCoordinateTransformContext());
        QgsRectangle newExtent = transformForExtentent.transformBoundingBox(oldExtent);
        return newExtent;
    }

    unsigned GISMapLayerManager::getLayerSize() {
        return userAddedLayers.size();
    }

    QgsLayerTree *GISMapLayerManager::getRoot() const {
        return root;
    }

    void GISMapLayerManager::setRoot(QgsLayerTree *root) {
        GISMapLayerManager::root = root;
    }

}