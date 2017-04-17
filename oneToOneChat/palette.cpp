#include "palette.h"
#include "ui_palette.h"

#include <QMoveEvent>
#include <QPainter>
#include <QDebug>
#include <QColorDialog>

#include "shape.h"

Palette::Palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Palette)
{
    ui->setupUi(this);
    mMouseIsPress = false;

    mPen.setColor(Qt::black);
    mPen.setWidth(2);
}

Palette::~Palette()
{
    delete ui;

    foreach(Shape *shape, mShapeStack)
    {
        delete shape;
    }
}

void Palette::setPenColor(const QColor &color)
{
    mPen.setColor(color);
}

QColor Palette::getPenColor()
{
	return mPen.color();
}

void Palette::revocation()
{
	if (mShapeStack.isEmpty())
	{
		return;
	}
	delete mShapeStack.last();
	mShapeStack.removeLast();
	update();
}

void Palette::cleanUp()
{
	foreach(Shape *shape, mShapeStack)
	{
		delete shape;
	}
	mShapeStack.clear();
	update();
}

void Palette::setIsDraw(bool isDraw)
{
	mIsDraw = isDraw;
	setMouseStyle(mIsDraw);
}

void Palette::setMouseStyle(bool isDraw)
{
	if (isDraw)
	{
		QCursor cursor;
		QPixmap pixmap(":/LoginWindow/images/pen.png");
		cursor = QCursor(pixmap, 0, pixmap.height());
		setCursor(cursor);
	}
	else
	{
		QCursor cursor;
		cursor = QCursor(Qt::ArrowCursor);
		setCursor(cursor);
	}
}

void Palette::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.setBrush(Qt::white);
    paint.setPen(Qt::white);   //绘制画板区域
    paint.drawRect(0, 0, size().width(), size().height());

    foreach(Shape *shape, mShapeStack)
    {
        shape->paint(paint);
    }
}

void Palette::mousePressEvent(QMouseEvent *event)
{
	if (!mIsDraw)
	{
		return;
	}
    switch (event->button()) {
    case Qt::LeftButton:
    {
        mMouseIsPress = true;
        Shape *shape = new Shape();
        mShapeStack.append(shape);
        shape->append(event->pos());
        shape->setPen(mPen);
        update();
    }
        break;
    default:
        break;
    }
}

void Palette::mouseMoveEvent(QMouseEvent *event)
{
	if (!mIsDraw)
	{
		return;
	}
    if(mMouseIsPress)
    {
        mShapeStack.last()->append(event->pos());
        update();
    }
}

void Palette::mouseReleaseEvent(QMouseEvent *event)
{
	if (!mIsDraw)
	{
		return;
	}
    if(Qt::LeftButton == event->button())
    {
        mMouseIsPress = false;
        update();
    }
}