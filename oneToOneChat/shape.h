#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QVector>
#include <QPen>

class QPainter;

typedef QPointF Point;
typedef QVector< Point > PointVec;

class Shape : public QObject
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);

	void append(const Point &point);

    void setPen(const QPen &pen);

	void paint(QPainter &painter, const QSize &size);

signals:

public slots:

private:
	PointVec mPointVec;

    QPen mPen;
};

#endif // SHAPE_H
