//
// Created by bugae on 19.08.2022.
//

#include <QPainter>
#include "CustomControlButton.h"

namespace gisUI {
    CustomControlButton::CustomControlButton(QWidget *parent) : QPushButton(parent) {}

    void CustomControlButton::mousePressEvent(QMouseEvent *e) {
        pressed = true;
        update();
    }

    void CustomControlButton::mouseReleaseEvent(QMouseEvent *e) {
        pressed = false;
        update();
        if (hovered)
            emit clicked();
    }

    void CustomControlButton::enterEvent(QEvent *event) {
        hovered = true;
        update();
    }

    void CustomControlButton::leaveEvent(QEvent *event) {
        hovered = false;
        update();
    }

    void CustomControlButton::paintEvent(QPaintEvent *e) {
        auto w = this->width();
        auto h = this->height();

        QPixmap _pix = this->icon().pixmap(this->iconSize()).scaled(w, h, Qt::KeepAspectRatio,
                                                                    Qt::TransformationMode::SmoothTransformation);
        QPainter *painter = new QPainter(this);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QRectF r = this->rect();
        painter->setPen(Qt::darkGray);
        painter->drawRoundRect(r, 25, 60);
        painter->setPen(Qt::NoPen);
        if (pressed) {
            _pix = _pix.scaled(_pix.width() * 0.8, _pix.height() * 0.8, Qt::KeepAspectRatio,
                               Qt::TransformationMode::SmoothTransformation);
        }
        painter->drawPixmap(w / 2 - _pix.width() / 2, h / 2 - _pix.height() / 2, _pix);
        painter->end();
    }
}