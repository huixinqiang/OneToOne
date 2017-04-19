#include "onetoonechat.h"
#include <QtWidgets/QApplication>

void registerMetaType()
{
	//ע�������   ���ڷ�Qt�����ͺͷǻ����������Ͳ���ʵ���źŲ�����
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<__int64>("__int64");
	qRegisterMetaType<int64_t>("int64_t");
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	registerMetaType();

	oneToOneChat w;
	w.show();
	return a.exec();
}
