//
// Created by bugae on 24.08.2022.
//

#include "DatabaseBrowser.h"
#include <QHeaderView>
#include <QGroupBox>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QPair>

namespace gisUI{

    DatabaseBrowser::DatabaseBrowser(QWidget *parent): QMainWindow(parent) {
        setWindowTitle("Просмотр/редактирование базы данных");
        planeTable = new QTableWidget;
        planeTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        shipTable = new QTableWidget;
        shipTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tabs = new QTabWidget;
        addTabItem(Type::PLANE,planeTable);
        addTabItem(Type::SHIP,shipTable);

        setCentralWidget(tabs);

    }

    void DatabaseBrowser::showEvent(QShowEvent *ev) {
        QMainWindow::showEvent(ev);
    }

    void DatabaseBrowser::addTabItem(Type type, QTableWidget* widget) {
        QGroupBox* groupBox = new QGroupBox;
        QString tabName = [&type] {
            if (type == Type::PLANE)
                return "Самолеты";
        else if (type == Type::SHIP)
                return "Корабли";}();

        tabs->addTab(groupBox,tabName);
        groupBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        QHBoxLayout* butLayout = new QHBoxLayout;
        QPushButton* addBut = new QPushButton("Добавить");
        addBut->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        connect(addBut, &QPushButton::clicked, this, [this, &type]{ addObject(type);});
        butLayout->addWidget(addBut);
        QPushButton* remBut = new QPushButton("Удалить");
        remBut->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        connect(addBut, &QPushButton::clicked, this, [this, &type]{ remObject(type);});
        butLayout->addWidget(remBut);
        QVBoxLayout* groupboxLay = new QVBoxLayout;
        groupboxLay->addLayout(butLayout);
        groupboxLay->addWidget(widget);
        groupBox->setLayout(groupboxLay);
    }

    void DatabaseBrowser::addObject(Type type) {
        AddFileDialog* dialog = new AddFileDialog(type);
        dialog->exec();
        delete dialog;
    }

    void DatabaseBrowser::remObject(Type type) {

    }

    AddFileDialog::AddFileDialog(Type _type):type(_type) {
        int nRow = 0;
        QGridLayout* lay = new QGridLayout;
        if (type == Type::PLANE){
            setWindowTitle(tr("Добавить аэродинамическую цель"));
            addItemToFileDialog(lay,"Имя",nRow++,true);
            lay->addWidget(new QLabel("Тип"), nRow, 0);
            QComboBox* types = new QComboBox;
            types->addItems({"Самолет", "Ракета"});
            lay->addWidget(types, nRow, 1);
            nRow++;
            addItemToFileDialog(lay,"Максимальная нормальная перегрузка",nRow++,true);
            addItemToFileDialog(lay,"Максимальная тагенсальная перегрузка",nRow++,true);
            addItemToFileDialog(lay,"Критический угол атаки",nRow++);
            addItemToFileDialog(lay,"Характеристическая площадь",nRow++);
            addItemToFileDialog(lay,"Практический потолок",nRow++,true);
            addItemToFileDialog(lay,"Масса пустого",nRow++);
            addItemToFileDialog(lay,"Максимальная масса внешней нагрузки",nRow++);
            addItemToFileDialog(lay,"Максимальная масса топлива",nRow++);
            addItemToFileDialog(lay,"Максимальное число Маха",nRow++,true);
            addItemToFileDialog(lay,"Максимальный скоростной напор",nRow++);
        } else if (type == Type::SHIP){
            setWindowTitle(tr("Добавить надводную цель"));
            addItemToFileDialog(lay,"Имя",nRow++,true);
            addItemToFileDialog(lay,"Водоизмещение",nRow++,true);
            addItemToFileDialog(lay,"Масса",nRow++,true);
            addItemToFileDialog(lay,"Длина",nRow++,true);
            addItemToFileDialog(lay,"Осадка",nRow++,true);
        }
        QPushButton* acceptButton = new QPushButton("Добавить");
        connect(acceptButton, &QPushButton::clicked, this, &AddFileDialog::tryAccept);
        QPushButton* rejectButton = new QPushButton("Отменить");
        lay->addWidget(acceptButton,nRow,0);
        lay->addWidget(rejectButton,nRow,1);
        setLayout(lay);
    }

    void AddFileDialog::addItemToFileDialog(QGridLayout* lay, QString name, int row, bool required) {
        lay->addWidget(new QLabel(name), row, 0);
        QLineEdit* line = new QLineEdit;
        if (required){
            line->setPlaceholderText("Обязательное поле");
            filled[name]=false;
            connect(line,&QLineEdit::textEdited, this, [this,name](QString text) { checkData(name,text);});
        }
        lay->addWidget(line,row,1);

    }

    void AddFileDialog::checkData(const QString &name, const QString &text) {
        if (!text.isEmpty())
            filled[name] = true;
        else
            filled[name] = false;
    }

    void AddFileDialog::tryAccept() {
        for (const auto& item: filled){
            if (!item)
                return;
        }
        QString name = dynamic_cast<QLineEdit*>(
                        dynamic_cast<QGridLayout*>(layout())->itemAtPosition(0,1)->widget())
                                ->text();

        checkNameRequest(name);
    }

    QVariant AddFileDialog::constructNewItem() {
        auto lay = dynamic_cast<QGridLayout *>(layout());
        auto getValue = [lay](int nRow) {
            auto text = dynamic_cast<QLineEdit *>
            (lay->itemAtPosition(nRow, 1)->widget())->text();
            if (text.isEmpty())
                text = QString::number(0);
            return text;
        };
        if (type == Type::PLANE) {
            PlaneCharcs plane;
            QString name = getValue(0);
            unsigned nRow = 1;
            plane.type = dynamic_cast<QComboBox *>(lay->itemAtPosition(nRow++, 1)->widget())->currentIndex();
            plane.maxNormalOverload = getValue(nRow++).toDouble();
            plane.maxTranverseOverload = getValue(nRow++).toDouble();
            plane.criticalAtackAngle = getValue(nRow++).toDouble();
            plane.surface = getValue(nRow++).toDouble();
            plane.maxHeight = getValue(nRow++).toDouble();
            plane.emptyMass = getValue(nRow++).toDouble();
            plane.maxExtLoadMass = getValue(nRow++).toDouble();
            plane.maxFuelMass = getValue(nRow++).toDouble();
            plane.maxMach = getValue(nRow++).toDouble();
            plane.maxSpeedPres = getValue(nRow++).toDouble();

            QPair<QString, PlaneCharcs> data{name, plane};
            qRegisterMetaType<PlaneCharcs>("PlaneCharcs");
            return QVariant::fromValue(data);
        } else if (type == Type::SHIP) {
            ShipCharcs ship;
            QString name = getValue(0);
            unsigned nRow = 1;
            ship.type = dynamic_cast<QComboBox *>(lay->itemAtPosition(nRow++, 1)->widget())->currentIndex();
            ship.displacement = getValue(nRow++).toDouble();
            ship.mass = getValue(nRow++).toDouble();
            ship.length = getValue(nRow++).toDouble();
            ship.draft = getValue(nRow++).toDouble();

            QPair<QString, ShipCharcs> data{name, ship};
            qRegisterMetaType<ShipCharcs>("ShipCharcs");
            return QVariant::fromValue(data);
        }
    }
}