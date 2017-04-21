#include "palette.h"
#include "ui_palette.h"

#include <QMoveEvent>
#include <QPainter>
#include <QDebug>
#include <QColorDialog>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "shape.h"

Palette::Palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Palette)
{
    ui->setupUi(this);
    mMouseIsPress = false;
	mDocPixmapZoom = true;

	mPixmap = NULL;

    mPen.setColor(Qt::black);
    mPen.setWidth(2);

	//initDocPixmapFromFileName("C:/Users/huixinqiang/Desktop/test.png");
	initDocPixmapFromUrl("http://img05.tooopen.com/images/20150531/tooopen_sy_127457023651.jpg");//http://img05.tooopen.com/images/20150531/tooopen_sy_127457023651.jpg
}

Palette::~Palette()
{
    delete ui;

	destroyDocPixmap();

	destroyShape();
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

void Palette::initDocPixmapFromFileName(const QString& fileName)
{
	destroyDocPixmap();

	mPixmap = new QPixmap(fileName);
}

void Palette::initDocPixmapFromUrl(const QString& url)
{
	destroyDocPixmap();

	QNetworkAccessManager manager;
	QEventLoop loop;

	QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
	//请求结束并下载完成后，退出子事件循环 
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环 
	loop.exec();

	QByteArray jpegData = reply->readAll();
	mPixmap = new QPixmap;
	if (mPixmap)
	{
		mPixmap->loadFromData(jpegData);
	}	
}

void Palette::destroyDocPixmap()
{
	if (mPixmap)
	{
		delete mPixmap;
		mPixmap = NULL;
	}
}

void Palette::setDocPixmapZoom(bool isZoom)
{
	mDocPixmapZoom = isZoom;
}

bool Palette::getDocPixmapZoom()
{
	return mDocPixmapZoom;
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

void Palette::destroyShape()
{
	foreach(Shape *shape, mShapeStack)
	{
		delete shape;
	}
}

void Palette::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
	QSize lSize = size();
	if (mPixmap && !mPixmap->isNull())
	{
		if (mDocPixmapZoom)
		{
			paint.drawPixmap(rect(), *mPixmap);
		}
		else
		{
			paint.drawPixmap(mPixmap->rect(), *mPixmap);
		}		
	}
	else
	{
		paint.setBrush(Qt::white);
		paint.setPen(Qt::white);   //绘制画板区域
		paint.drawRect(0, 0, lSize.width(), lSize.height());
	}    

    foreach(Shape *shape, mShapeStack)
    {
        shape->paint(paint, lSize);
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

		QPoint point = event->pos();
		QSize lSize = size();
		QPointF pointf(point.x() * 1.0 / lSize.width(), point.y() * 1.0 / lSize.height());

		mShapeStack.last()->append(pointf);
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
		QPoint point = event->pos();
		QSize lSize = size();
		QPointF pointf(point.x() * 1.0 / lSize.width(), point.y() * 1.0 / lSize.height());

		mShapeStack.last()->append(pointf);
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