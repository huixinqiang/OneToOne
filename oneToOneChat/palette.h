#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QStack>
#include <QPen>

namespace Ui {
class Palette;
}

class Shape;

class Palette : public QWidget
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = 0);
    ~Palette();

    void setPenColor(const QColor &color);		//设置画笔颜色
	QColor getPenColor();						//获取当前画笔颜色

	void revocation();							//撤销上一次操作
	void cleanUp();								//清空白板

	void setIsDraw(bool isDraw);				//设置是否需要画

	void initDocPixmapFromFileName(const QString& fileName);	//根据文件名初始化文档图片
	void initDocPixmapFromUrl(const QString& url);				//根据url初始化文档图片
	void destroyDocPixmap();						//销毁文档图片

	void setDocPixmapZoom(bool isZoom);				//设置文档图片是否缩放
	bool getDocPixmapZoom();						//获取文档图片是否缩放

private:
	void setMouseStyle(bool isDraw);			//设置鼠标指针样式
	void destroyShape();						//销毁所有操作过的笔画

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Palette *ui;

    QStack< Shape* > mShapeStack;				//笔画数据

    bool mMouseIsPress;							//鼠标是否按下
	bool mIsDraw;								//是否需要画
	bool mDocPixmapZoom;						//图片是否缩放   true：根据白板大小缩放图片   false：根据图片实际大小显示   默认为true
    QPen mPen;

	QPixmap* mPixmap;							//文档图片
};

#endif // PALETTE_H
