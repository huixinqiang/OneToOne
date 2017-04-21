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

	void initDocPixmapFromFileName(const QString& fileName);	//�����ļ�����ʼ���ĵ�ͼƬ
	void initDocPixmapFromUrl(const QString& url);				//����url��ʼ���ĵ�ͼƬ
	void destroyDocPixmap();						//�����ĵ�ͼƬ

	void setDocPixmapZoom(bool isZoom);				//�����ĵ�ͼƬ�Ƿ�����
	bool getDocPixmapZoom();						//��ȡ�ĵ�ͼƬ�Ƿ�����

private:
	void setMouseStyle(bool isDraw);			//�������ָ����ʽ
	void destroyShape();						//�������в������ıʻ�

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Palette *ui;

    QStack< Shape* > mShapeStack;				//�ʻ�����

    bool mMouseIsPress;							//����Ƿ���
	bool mIsDraw;								//�Ƿ���Ҫ��
	bool mDocPixmapZoom;						//ͼƬ�Ƿ�����   true�����ݰװ��С����ͼƬ   false������ͼƬʵ�ʴ�С��ʾ   Ĭ��Ϊtrue
    QPen mPen;

	QPixmap* mPixmap;							//�ĵ�ͼƬ
};

#endif // PALETTE_H
