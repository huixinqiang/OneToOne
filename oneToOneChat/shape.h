#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QVector>
#include <QPen>

class QPainter;

class Shape : public QObject
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);

    void append(const QPoint &point);

    void setPen(const QPen &pen);

    void paint(QPainter &painter);

signals:

public slots:

private:
    QVector< QPoint > mPointVec;

    QPen mPen;
};

#endif // SHAPE_H
