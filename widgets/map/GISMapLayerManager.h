//
// Created by bugae on 19.08.2022.
//

#ifndef GIS_GUI_GISMAPLAYERMANAGER_H
#define GIS_GUI_GISMAPLAYERMANAGER_H

#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgsvectorlayer.h>

#include <QObject>
#include <qgslayertree.h>

namespace gisUI {
    struct Coordinates{
        QgsCoordinateReferenceSystem mainCrs;
        QgsCoordinateReferenceSystem currentCrs;
        QgsCoordinateReferenceSystem wgs84;
        Coordinates();
    };

    struct CoordinateTransform{
        QgsCoordinateTransform wgs84ToMainCrs; ///< Преобразование из WGS84 в главную систему координат
        QgsCoordinateTransform mainCrsToWgs84; ///< Преобразование из главной системы координат в WGS84
        QgsCoordinateTransform currentCrsToWgs84; ///< Преобразование текущей системы координат в WGS84
        CoordinateTransform();
    };

    class GISMapLayerManager : public QObject {
        Q_OBJECT
    public:
        GISMapLayerManager();
        QgsCoordinateReferenceSystem getMainCRS();
        QgsCoordinateReferenceSystem getCurrentCRS();
        QMap<QString, QString> getLayersChecksum();
        unsigned getLayerSize();

    public slots:
        void setNewVectorLayers(QList<QgsVectorLayer*> layers);
    private:
        Coordinates coords;
        CoordinateTransform transforms;
        QList<QgsMapLayer*> userAddedLayers;
        QgsLayerTree *root;
    public:
        QgsLayerTree *getRoot() const;

        void setRoot(QgsLayerTree *root);

    private:

        QgsRectangle getLayerExtentInMainCrs(const QgsCoordinateReferenceSystem &crs, const QgsRectangle &oldExtent);
        QgsRectangle getLayerExtentInWgs84(const QgsCoordinateReferenceSystem &crs, const QgsRectangle &oldExtent);
        QgsRectangle getLayerExtentInCurrentCrs(const QgsCoordinateReferenceSystem &crs, const QgsRectangle &oldExtent);
        QgsRectangle getLayerExtentInCrs(const QgsCoordinateReferenceSystem &crs, const QgsCoordinateReferenceSystem &dstCrs,
                                         const QgsRectangle &oldExtent);

        void setMainCrs(const QgsCoordinateReferenceSystem& crs);


        void createAuxiliaryLayers();
        signals:
        void setLayersRequest();
        void sendLayers(QList<QgsMapLayer*> layers);
        void sendExtent(QgsRectangle rect);
        void sendDestCoords(QgsCoordinateReferenceSystem crs);
    };
}


#endif //GIS_GUI_GISMAPLAYERMANAGER_H
