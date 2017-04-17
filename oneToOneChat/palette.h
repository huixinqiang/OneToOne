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

private:
	void setMouseStyle(bool isDraw);			//设置鼠标指针样式

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Palette *ui;

    QStack< Shape * > mShapeStack;

    bool mMouseIsPress;							//鼠标是否按下
	bool mIsDraw;								//是否需要画
    QPen mPen;
};

#endif // PALETTE_H
