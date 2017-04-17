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

    void setPenColor(const QColor &color);		//���û�����ɫ
	QColor getPenColor();						//��ȡ��ǰ������ɫ

	void revocation();							//������һ�β���
	void cleanUp();								//��հװ�

	void setIsDraw(bool isDraw);				//�����Ƿ���Ҫ��

private:
	void setMouseStyle(bool isDraw);			//�������ָ����ʽ

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Palette *ui;

    QStack< Shape * > mShapeStack;

    bool mMouseIsPress;							//����Ƿ���
	bool mIsDraw;								//�Ƿ���Ҫ��
    QPen mPen;
};

#endif // PALETTE_H
