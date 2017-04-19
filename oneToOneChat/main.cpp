#include "onetoonechat.h"
#include <QtWidgets/QApplication>

void registerMetaType()
{
	//注册该类型   用于非Qt类类型和非基本数据类型参数实现信号槽连接
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
