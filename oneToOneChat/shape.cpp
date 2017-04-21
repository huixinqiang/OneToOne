#include "shape.h"

#include <QPainter>
#include <QDebug>

Shape::Shape(QObject *parent) : QObject(parent)
{

}

void Shape::append(const Point &point)
{
    mPointVec.append(point);
}

void Shape::setPen(const QPen &pen)
{
    mPen = pen;
}

void Shape::paint(QPainter &painter, const QSize &size)
{
    painter.setPen(mPen);
    int vecSize = mPointVec.size();
    if(vecSize <= 0)
    {
        return;
    }
    else if(vecSize == 1)
    {
        painter.drawPoint(mPointVec.first());
    }
    else
    {
        for(int i = 0; i < vecSize - 1; i++)
        {
			const QPointF &pointf1 = mPointVec.at(i);
			const QPointF &pointf2 = mPointVec.at(i + 1);
			int width = size.width();
			int height = size.height();
			painter.drawLine(pointf1.x() * width, pointf1.y() * height, pointf2.x() * width, pointf2.y() * height);
        }
    }
}
