//
// Created by bugae on 23.08.2022.
//

#ifndef GIS_GUI_DATABASEWORKER_H
#define GIS_GUI_DATABASEWORKER_H
#include <QString>
#include <QMap>
#include <QSqlDatabase>


namespace gisUI {

#define PLANE_FIELDS \
    NAMES_MACRO(int,type)\
    NAMES_MACRO(double,maxNormalOverload)\
    NAMES_MACRO(double,maxTranverseOverload)\
    NAMES_MACRO(double,criticalAtackAngle)\
    NAMES_MACRO(double,surface)\
    NAMES_MACRO(double,maxHeight)\
    NAMES_MACRO(double,emptyMass)\
    NAMES_MACRO(double,maxExtLoadMass)\
    NAMES_MACRO(double,maxFuelMass)\
    NAMES_MACRO(double,maxMach)\
    NAMES_MACRO(double,maxSpeedPres)

#define SHIP_FIELDS \
    NAMES_MACRO(int,type)\
    NAMES_MACRO(double,displacement)\
    NAMES_MACRO(double,mass)\
    NAMES_MACRO(double,length)\
    NAMES_MACRO(double,draft)


    struct PlaneCharcs {
        #define NAMES_MACRO(type,name) type name;
            PLANE_FIELDS
        #undef NAMES_MACRO
    };


    struct ShipCharcs {
        #define NAMES_MACRO(type,name) type name;
                SHIP_FIELDS
        #undef NAMES_MACRO
    };

    enum class Type {PLANE, SHIP, SCENARIO, NONE};
    class DatabaseWorker {
    public:
        explicit DatabaseWorker(const QString &dbType = "QSQLITE");
        ~DatabaseWorker();

        void open(const QString &dbName = "default.db");
        void add(const QString& name, const PlaneCharcs& plane);
        void add(const QString& name, const ShipCharcs& ship);
        void edit(const QString& name, const PlaneCharcs& plane);
        void edit(const QString& name, const ShipCharcs& ship);
        void remove(const QString& name, Type type);
        void remove(const unsigned& id, Type type);
        void write();

    private:
        QSqlDatabase db;
        QMap<QString, unsigned> planesNameToId;
        QMap<unsigned, PlaneCharcs> planesIdToChars;
        QMap<QString, unsigned> shipsNameToId;
        QMap<unsigned, ShipCharcs> shipsIdToChars;

        void createTable(Type type=Type::NONE);
        void writeDataInDB(Type type=Type::NONE);
        void insertData(QString& prep, Type type=Type::NONE);

        void readPlanes();
        void readShips();
        void writePlanes();
        void writeShips();
    };
}

#endif //GIS_GUI_DATABASEWORKER_H
