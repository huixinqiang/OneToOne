#include "onetoonechat.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qRegisterMetaType<std::string>("std::string"); //◊¢≤·∏√¿‡–Õ
	qRegisterMetaType<__int64>("__int64");
	oneToOneChat w;
	w.show();
	return a.exec();
}
