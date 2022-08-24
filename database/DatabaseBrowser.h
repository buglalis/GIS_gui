//
// Created by bugae on 24.08.2022.
//

#ifndef GIS_GUI_DATABASEBROWSER_H
#define GIS_GUI_DATABASEBROWSER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDialog>
#include <QGridLayout>
#include "DatabaseWorker.h"

namespace gisUI {

    class AddFileDialog:public QDialog{
        Q_OBJECT
    public:
        explicit AddFileDialog(Type _type);
    private:
        const Type type;
        QMap<QString, bool> filled;
        void addItemToFileDialog(QGridLayout* lay, QString name, int row, bool required = false);
        QVariant constructNewItem();
    private slots:
        void checkData(const QString& name, const QString& text);
        void tryAccept();
    signals:
        void checkNameRequest(QString name);
    };

    class DatabaseBrowser: public QMainWindow{
        Q_OBJECT
    public:
        explicit DatabaseBrowser(QWidget* parent = nullptr);
    private:
        QTableWidget* planeTable;
        QTableWidget* shipTable;
        QTabWidget* tabs;
        void showEvent(QShowEvent* ev) override;

    private slots:
        void addTabItem(Type type, QTableWidget* widget);

        void addObject(Type type);
        void remObject(Type type);

    };
}

Q_DECLARE_METATYPE(gisUI::PlaneCharcs)
Q_DECLARE_METATYPE(gisUI::ShipCharcs)


#endif //GIS_GUI_DATABASEBROWSER_H
