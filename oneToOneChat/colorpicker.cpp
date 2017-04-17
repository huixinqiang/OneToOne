#include "colorpicker.h"

ColorPicker::ColorPicker(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::setCurColor(const QColor &curColor)
{
	mCurColor = curColor;
}

void ColorPicker::on_pushButton_black_clicked()
{
	mCurColor = Qt::black;
	emit selectColor(mCurColor);
	close();
}

void ColorPicker::on_pushButton_yellow_clicked()
{
	mCurColor = QColor(255, 204, 0);
	emit selectColor(mCurColor);
	close();
}

void ColorPicker::on_pushButton_purple_clicked()
{
	mCurColor = QColor(255, 51, 255);
	emit selectColor(mCurColor);
	close();
}

void ColorPicker::on_pushButton_green_clicked()
{
	mCurColor = QColor(0, 153, 0);
	emit selectColor(mCurColor);
	close();
}

void ColorPicker::on_pushButton_blue_clicked()
{
	mCurColor = QColor(0, 153, 255);
	emit selectColor(mCurColor);
	close();
}

void ColorPicker::on_pushButton_red_clicked()
{
	mCurColor = QColor(204, 0, 0);
	emit selectColor(mCurColor);
	close();
}