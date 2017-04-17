#include "onetoonechat.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include "IMInterface.h"
#include "UIWindowSet_oneToOne.h"

#define APPKEY "4fe3a3fba0a40a00daf011049a29d995" //"4fe3a3fba0a40a00daf011049a29d995"

#define USERNAME "5f9ba6b45c8003493fb5bee6a2d59ef7"
#define PASSWORD "5d15453f7b8dd75242a5fdfc0662ff47"

QLabel *mStatus;
UIWindowSet_oneToOne *mUIWindowSet_oneToOne;
oneToOneChat *mOneToOneChat;

oneToOneChat::oneToOneChat(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	app_key_ = APPKEY;
	mStatus = ui.label_status;
	mUIWindowSet_oneToOne = NULL;
	mOneToOneChat = this;

	//RequestKey();
	initNim();
	OnInitDialog();
	createWhiteBoard();
}

oneToOneChat::~oneToOneChat()
{
	if (mUIWindowSet_oneToOne)
	{
		delete mUIWindowSet_oneToOne;
		mUIWindowSet_oneToOne = NULL;
	}

	destroyNim();
}

void oneToOneChat::initNim()
{
	//初始化云信SDK
	nim::SDKConfig sdk_config;
	//设置数据库密钥前缀
	sdk_config.database_encrypt_key_ = "OneToOneChat";
	//初始化IM SDK
	qDebug() << nim::Client::Init(app_key_, "", "", sdk_config);
}

void oneToOneChat::destroyNim()
{
	//清理云信SDK
	//清理IM SDK
	nim::Client::Cleanup();
}

bool oneToOneChat::OnInitDialog()
{
	// 注册云信全局广播通知的监听
	nim::Client::RegKickoutCb(&CallbackKickout);
	nim::Client::RegDisconnectCb(&CallbackDisconnect);
	nim::Client::RegKickOtherClientCb(&CallbackKickOther);
	nim::Client::RegMultispotLoginCb(&CallbackMultiSpotLogin);
	nim::Client::RegReloginCb(&CallbackLogin);

	nim::Talk::RegReceiveCb(&CallbackReceiveMsg);
	nim::Talk::RegReceiveMessagesCb(&CallbackReceiveMsgs);
	nim::Talk::RegSendMsgCb(&CallbackSendMsgArc);

	return true;
}

void oneToOneChat::createWhiteBoard()
{
	if (mUIWindowSet_oneToOne)
	{
		delete mUIWindowSet_oneToOne;
	}
	mUIWindowSet_oneToOne = new UIWindowSet_oneToOne();
}

void oneToOneChat::initOneToOne()
{
	mUIWindowSet_oneToOne->createRtsRoom(mRoomName, mRoomInfo);
}

void oneToOneChat::on_pushButton_login_clicked()
{
	mRoomName = ui.lineEdit_user->text();
	mRoomInfo = ui.lineEdit_password->text();

	if (mRoomName.isEmpty())
	{
		QMessageBox::warning(this, "提示", "房间名不能为空");
		return;
	}

	QString username = USERNAME;
	QString password = PASSWORD;

	if (nim::Client::Login(app_key_, username.toStdString(), password.toStdString(), &CallbackLogin))//nim::Tool::GetMd5(password.toStdString())
	{
		//mWhiteBoard->show();
		mUIWindowSet_oneToOne->show();
	}
}

void oneToOneChat::on_pushButton_logout_clicked()
{
	OnBnClickedButtonLogout();
}

void oneToOneChat::on_pushButton_cancel_clicked()
{
	OnBnClickedButtonExit();
}

//注销
void oneToOneChat::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码

	nim::Client::Logout(nim::kNIMLogoutChangeAccout, [](nim::NIMResCode code) {
		printf("Logout, rescode : %d.\r\n", code);
		mStatus->setText(QString("Logout, rescode : %1.").arg(code));
	});
}

//退出
void oneToOneChat::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码

	nim::Client::Logout(nim::kNIMLogoutAppExit, [](nim::NIMResCode code) {
		printf("Exit, rescode : %d.\r\n", code);
		mStatus->setText(QString("Exit, rescode : %1.").arg(code));
	});
}

void CallbackLogin(const nim::LoginRes &res)
{
	if (res.login_step_ == nim::kNIMLoginStepLogin)
	{
		printf("Login result : %d, %s.\r\n", res.res_code_, res.res_code_ == nim::kNIMResSuccess ? "succeed" : "failed");
		mStatus->setText(QString("Login result : %1, %2.").arg(res.res_code_).arg(res.res_code_ == nim::kNIMResSuccess ? "succeed" : "failed"));
		if (res.res_code_ == nim::kNIMResSuccess)
		{
			mOneToOneChat->initOneToOne();
		}
	}
	else
	{
		printf("Logining %d step...\r\n", res.login_step_);
		mStatus->setText(QString::number(res.login_step_));
	}
}

void CallbackReceiveMsgs(const std::list<nim::IMMessage>& msgs)
{

}

void CallbackReceiveMsg(const nim::IMMessage& msg)
{

}

void CallbackSendMsgArc(const nim::SendMessageArc& arc)
{

}

void CallbackKickOther(const nim::KickOtherRes& res)
{

}

void CallbackMultiSpotLogin(const nim::MultiSpotLoginRes& res)
{

}

void CallbackDisconnect()
{

}

void CallbackKickout(const nim::KickoutRes& res)
{

}