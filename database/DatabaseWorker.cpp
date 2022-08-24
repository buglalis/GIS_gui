//
// Created by bugae on 23.08.2022.
//

#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include "DatabaseWorker.h"


namespace gisUI {

    QString SQLtype(const QString type){
        if(type == "int")
            return "INTEGER";
        if(type == "double" || type == "float")
            return "REAL";
        if(type == "QString")
            return "TEXT";
    }

    DatabaseWorker::DatabaseWorker(const QString &dbType) {
        db = QSqlDatabase::addDatabase(dbType);
    }

    void DatabaseWorker::open(const QString &dbName) {
        db.setDatabaseName(dbName);
        if(db.open()){
            readPlanes();
            readShips();
        }
        qDebug()<<"opened";
        write();
    }

    void DatabaseWorker::readPlanes() {
        db.transaction();
        createTable(Type::PLANE);
        QSqlQuery query;
        query.exec("SELECT * FROM Planes");
        QSqlRecord rec = query.record();
        PlaneCharcs plane;
        while (query.next()) {
            #define NAMES_MACRO(type, name) plane.name = query.value(rec.indexOf(#name)).toDouble();
                PLANE_FIELDS
            #undef NAMES_MACRO
            unsigned id = query.value(rec.indexOf("Id")).toUInt();
            QString name = query.value(rec.indexOf("Name")).toString();
            planesIdToChars[id] = plane;
            planesNameToId[name] = id;
        }
    }

    void DatabaseWorker::readShips() {
        db.transaction();
        createTable(Type::SHIP);
        QSqlQuery query;
        query.exec("SELECT * FROM Ships");
        QSqlRecord rec = query.record();
        ShipCharcs ship;
        while (query.next()) {
        #define NAMES_MACRO(type, name) ship.name = query.value(rec.indexOf(#name)).toDouble();
            SHIP_FIELDS
        #undef NAMES_MACRO
            unsigned id = query.value(rec.indexOf("Id")).toUInt();
            QString name = query.value(rec.indexOf("Name")).toString();
            shipsIdToChars[id] = ship;
            shipsNameToId[name] = id;
        }
        if (!db.commit())
            qDebug()<<"Failed to commit transaction";
    }

    void DatabaseWorker::add(const QString &name, const PlaneCharcs &plane) {
        if (name.isEmpty()) {
            qDebug() << "Message can`t be empty"; //ToDO:: print into message box
            return;
        }
        if (!db.isOpen()) {
            qDebug() << "DB isn`t open"; //ToDO:: print into message box
            return;
        }
        if (planesNameToId.find(name)!=planesNameToId.end()){
            qDebug() << QString("Plane with name %1 already exists").arg(name); //ToDO:: print into message box
            return;
        }

        unsigned newId = planesIdToChars.lastKey() + 1;
        planesNameToId[name] = newId;
        planesIdToChars[newId] = plane;
    }

    void DatabaseWorker::add(const QString &name, const ShipCharcs &ship) {
        if (name.isEmpty()) {
            qDebug() << "Message can`t be empty"; //ToDO:: print into message box
            return;
        }
        if (!db.isOpen()) {
            qDebug() << "DB isn`t open"; //ToDO:: print into message box
            return;
        }
        if (shipsNameToId.find(name)!=shipsNameToId.end()){
            qDebug() << QString("Ship with name %1 already exists").arg(name); //ToDO:: print into message box
            return;
        }

        unsigned newId = shipsIdToChars.lastKey() + 1;
        shipsNameToId[name] = newId;
        shipsIdToChars[newId] = ship;
    }



    void DatabaseWorker::edit(const QString &name, const PlaneCharcs &plane) {
        remove(name, Type::PLANE);
        add(name,plane);
    }

    void DatabaseWorker::edit(const QString &name, const ShipCharcs &ship) {
        remove(name, Type::SHIP);
        add(name,ship);
    }

    void DatabaseWorker::remove(const QString &name, Type type) {
        if (name.isEmpty()) {
            qDebug() << "Message can`t be empty"; //ToDO:: print into message box
            return;
        }
        if (!db.isOpen()) {
            qDebug() << "DB isn`t open"; //ToDO:: print into message box
            return;
        }
        if (type == Type::PLANE) {
            if (planesNameToId.find(name) == planesNameToId.end()) {
                qDebug() << QString("Plane with name %1 not exist. Can`t remove").arg(name); //ToDO:: print into message box
                return;
            }
            auto idDel = planesNameToId[name];
            planesNameToId.remove(name);
            planesIdToChars.remove(idDel);
        } else if (type == Type::SHIP) {
            if (shipsNameToId.find(name) == shipsNameToId.end()) {
                qDebug() << QString("Ship with name %1 not exist. Can`t remove").arg(name); //ToDO:: print into message box
                return;
            }
            auto idDel = shipsNameToId[name];
            shipsNameToId.remove(name);
            shipsIdToChars.remove(idDel);
        } else
            return;
    }

    void DatabaseWorker::write() {
        if (!db.isOpen()) {
            qDebug() << "DB isn`t open"; //ToDO:: print into message box
            return;
        }
        db.transaction();
        writePlanes();
        writeShips();
        qDebug()<<"Transaction status is "<< db.commit();
    }



    void DatabaseWorker::remove(const unsigned int &id, Type type) {
        if (!db.isOpen()) {
            qDebug() << "DB isn`t open"; //ToDO:: print into message box
            return;
        }
        if (type == Type::PLANE) {
            if (planesIdToChars.find(id) == planesIdToChars.end()) {
                qDebug() << QString("Plane with id %1 not exist. Can`t remove").arg(id); //ToDO:: print into message box
                return;
            }
            const auto nameDel = [=] {
                QMapIterator<QString, unsigned> it(planesNameToId);
                while (it.hasNext()){
                    it.next();
                    if (it.value() == id)
                        return it.key();
                }
                return QString();
            }();
            if (nameDel.isEmpty())
                return;
            planesNameToId.remove(nameDel);
            planesIdToChars.remove(id);
        } else if (type == Type::SHIP) {
            if (shipsIdToChars.find(id) == shipsIdToChars.end()) {
                qDebug() << QString("Ship with id %1 not exist. Can`t remove").arg(id); //ToDO:: print into message box
                return;
            }
            const auto nameDel = [=] {
                QMapIterator<QString, unsigned> it(shipsNameToId);
                while (it.hasNext()){
                    it.next();
                    if (it.value() == id)
                        return it.key();
                }
                return QString();
            }();
            if (nameDel.isEmpty())
                return;
            shipsNameToId.remove(nameDel);
            shipsIdToChars.remove(id);
        } else
            return;
    }

    DatabaseWorker::~DatabaseWorker() {
        db.close();
    }

    void DatabaseWorker::createTable(Type type) {
        QString tableName;
        if (type == Type::PLANE)
            tableName = "Planes";
        else if (type == Type::SHIP)
            tableName = "Ships";
        else
            return;
        QSqlQuery query;
        QString str = QString("DROP TABLE IF EXISTS %1;").arg(tableName);
        query.exec(str);
        str = QString("CREATE TABLE %1(Id INTEGER PRIMARY KEY, Name TEXT").arg(tableName);
        if (type == Type::PLANE) {
            #define NAMES_MACRO(type,name) str += ","; str += #name; str += " "; str += SQLtype(#type);
                PLANE_FIELDS
            #undef NAMES_MACRO
        }
        else if (type == Type::SHIP) {
            #define NAMES_MACRO(type, name) str += ","; str += #name; str += " "; str += SQLtype(#type);
                        SHIP_FIELDS
            #undef NAMES_MACRO
        }
        str += ");";
        query.exec(str);
    }

    void DatabaseWorker::insertData(QString &prep, Type type) {

        QMapIterator<QString, unsigned> *it;
        if (type==Type::PLANE)
            it = new QMapIterator<QString, unsigned>(planesNameToId);
        else if (type==Type::SHIP)
            it = new QMapIterator<QString, unsigned>(shipsNameToId);
        else return;
        QSqlQuery query;
        while (it->hasNext()) {
            it->next();
            query.prepare(prep);
            query.bindValue(":Id", QString::number(it->value()));
            query.bindValue(":Name", it->key());
            if (type==Type::PLANE) {
                #define NAMES_MACRO(type, name) query.bindValue(QString(":")+QString(#name),QString::number(planesIdToChars[it->value()].name));
                                PLANE_FIELDS
                #undef NAMES_MACRO
            } else if (type==Type::SHIP) {
                #define NAMES_MACRO(type, name) query.bindValue(QString(":")+QString(#name),QString::number(shipsIdToChars[it->value()].name));
                                SHIP_FIELDS
                #undef NAMES_MACRO
            }
            query.exec();
        }
        delete it;
    }

    void DatabaseWorker::writeDataInDB(Type type) {
        createTable(type);
        QString prep;
        if (type == Type::PLANE) {
            prep = "INSERT INTO Planes VALUES (:Id, :Name";
            #define NAMES_MACRO(type, name) prep += ",:"; prep += #name;
                PLANE_FIELDS
            #undef NAMES_MACRO
        } else if (type == Type::SHIP) {
            prep = "INSERT INTO Ships VALUES (:Id, :Name";
            #define NAMES_MACRO(type, name) prep += ",:"; prep += #name;
                SHIP_FIELDS
            #undef NAMES_MACRO
        }
        prep += ");";
        insertData(prep,type);
    }

    void DatabaseWorker::writePlanes() {
        writeDataInDB(Type::PLANE);
    }

    void DatabaseWorker::writeShips() {
        writeDataInDB(Type::SHIP);
    }
}