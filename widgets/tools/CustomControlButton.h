//
// Created by bugae on 19.08.2022.
//

#ifndef GIS_GUI_CUSTOMCONTROLBUTTON_H
#define GIS_GUI_CUSTOMCONTROLBUTTON_H

#include <QPushButton>

namespace gisUI {
    class CustomControlButton : public QPushButton {
    Q_OBJECT

    public:
        CustomControlButton(QWidget *parent = nullptr);
        void mousePressEvent(QMouseEvent *e) override;
        void mouseReleaseEvent(QMouseEvent *e) override;
        void enterEvent(QEvent *event) override;
        void leaveEvent(QEvent *event) override;
        void paintEvent(QPaintEvent *e) override;

    private:
        bool hovered = false;
        bool pressed = false;
    };
}
#endif //GIS_GUI_CUSTOMCONTROLBUTTON_H
