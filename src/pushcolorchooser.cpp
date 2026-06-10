#include <QPainter>
#include "pushcolorchooser.h"

PushColorChooser::PushColorChooser(QWidget *parent) :
    QPushButton(parent)
{
}

void PushColorChooser::setColor(QColor color)
{
    if (color.isValid()) {
        QPixmap pixmap(30,20);
        pixmap.fill(QColor("black"));

        QPainter painter(&pixmap);
        painter.setBrush(QBrush(color));
        painter.drawRect(0, 0, 30, 20);

        setIcon(pixmap);
    }
}
