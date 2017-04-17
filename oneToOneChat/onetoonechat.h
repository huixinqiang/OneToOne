#ifndef ONETOONECHAT_H
#define ONETOONECHAT_H

#include <QtWidgets/QMainWindow>
#include "ui_onetoonechat.h"

#include <QNetworkAccessManager>

//IM SDK�ӿڶ���ͷ�ļ�
#include "nim_cpp_api.h"
#include "nim_cpp_client.h"

//ǰ������������Ϊ�յ��������߻�ͬ����Ϣ֪ͨ�������յ�������Ϣ֪ͨ������������Ϣ���֪ͨ������������֪ͨ��������˵�¼֪ͨ����������֪ͨ����������֪ͨ��������¼����֪ͨ������
void CallbackReceiveMsgs(const std::list<nim::IMMessage>& msgs);
void CallbackReceiveMsg(const nim::IMMessage& msg);
void CallbackSendMsgArc(const nim::SendMessageArc& arc);
void CallbackKickOther(const nim::KickOtherRes& res);
void CallbackMultiSpotLogin(const nim::MultiSpotLoginRes& res);
void CallbackDisconnect();
void CallbackKickout(const nim::KickoutRes& res);
void CallbackLogin(const nim::LoginRes& res);

class QNetworkReply;
class IMInterface;

class oneToOneChat : public QMainWindow
{
	Q_OBJECT

public:
	oneToOneChat(QWidget *parent = 0);
	~oneToOneChat();

	void initNim();
	void destroyNim();

	bool OnInitDialog();

	void createWhiteBoard();

	void initOneToOne();

private slots:
void on_pushButton_login_clicked();
void on_pushButton_logout_clicked();
void on_pushButton_cancel_clicked();
void OnBnClickedButtonLogout();
void OnBnClickedButtonExit();

private:
	Ui::oneToOneChatClass ui;

	std::string app_key_;
	QString mRemeberToken;

	QString mRoomName;
	QString mRoomInfo;

	QNetworkReply *reply;
	QNetworkAccessManager manager;
};

#endif // ONETOONECHAT_H
