#include "shape.h"

#include <QPainter>
#include <QDebug>

Shape::Shape(QObject *parent) : QObject(parent)
{

}

void Shape::append(const QPoint &point)
{
    mPointVec.append(point);
}

void Shape::setPen(const QPen &pen)
{
    mPen = pen;
}

void Shape::paint(QPainter &painter)
{
    painter.setPen(mPen);
    int size = mPointVec.size();
    if(size <= 0)
    {
        return;
    }
    else if(size == 1)
    {
        painter.drawPoint(mPointVec.first());
    }
    else
    {
        for(int i = 0; i < size - 1; i++)
        {
            painter.drawLine(mPointVec.at(i), mPointVec.at(i + 1));
        }
    }
}
