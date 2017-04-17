﻿#include "IMInterface.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#define ERROR_INFO(code, error_string) QString("%1	错误代码：%2").arg(error_string).arg(code)

using namespace nim;

IMInterface *IMInterface::mInstace = NULL;

IMInterface *IMInterface::getInstance()
{
	if (!mInstace)
	{
		mInstace = new IMInterface();
	}

	return mInstace;
}

void IMInterface::destroy()
{
	if (mInstace)
	{
		delete mInstace;
		mInstace = NULL;
	}
}

IMInterface::IMInterface(QObject *parent)
	: QObject(parent)
{
	mVChatInitSuccess = false;

	mLastError = "无错误";
}

IMInterface::~IMInterface()
{
	destroyRts();

	destroyVChat();
}

void IMInterface::initWhiteBoard()
{
	initRtsCallback();
}

void IMInterface::destroyRts()
{
	if (!mRtsSession_id.empty())
	{
		Rts::Hangup(mRtsSession_id, &CallbackHangup);
		mRtsSession_id = "";
	}	
}

void IMInterface::initVChat()
{
	mVChatInitSuccess = VChat::Init("");
}

void IMInterface::destroyVChat()
{
	if (mVChatInitSuccess)
	{
		VChat::Cleanup();
	}	
}

void IMInterface::initRtsCallback()
{
	Rts::SetStartNotifyCb(&CallbackStartNotify);
	Rts::SetConnectNotifyCb(&CallbackConnectNotify);
	Rts::SetAckNotifyCb(&CallbackAckNotify);
	Rts::SetMemberChangeCb(&CallbackMemberNotify);
	Rts::SetHangupNotifyCb(&CallbackHangupNotify);
	Rts::SetRecDataCb(&CallbackRecData);
	Rts::SetControlNotifyCb(&CallbackControlNotify);
	Rts::SetSyncAckNotifyCb(&CallbackSyncAckNotify);
}

//void IMInterface::createRtsRoom(int channel_type, const QString& uid, const QString& apns, const QString& custom_info, bool data_record, bool audio_record)
//{
//	Rts::StartChannel(channel_type, uid.toStdString(), apns.toStdString(), custom_info.toStdString(), data_record, audio_record, &CallbackStartChannel);
//}

void IMInterface::createRtsRoom(const std::string &name, const std::string &custom_info/* = ""*/)
{
	mRtsRoomName = name;
	Rts::CreateConf(mRtsRoomName, custom_info, &CallbackCreateConf);
}

void IMInterface::joinRtsRoom(const std::string &name, bool record/* = false*/)
{
	mRtsRecord = record;
	Rts::JoinConf(name, record, &CallbackJoinConf);
}

void IMInterface::SendData(const std::string& session_id, int channel_type, const std::string& data, const std::string& uid /* = "" */)
{
	Rts::SendData(session_id, channel_type, data, uid);
}

void IMInterface::setRtsSessionID(const std::string &sessionID)
{
	mRtsSession_id = sessionID;
}

const std::string &IMInterface::getRtsSessionID()
{
	return mRtsSession_id;
}

void IMInterface::setRtsRoomName(const std::string &roomName)
{
	mRtsRoomName = roomName;
}

const std::string &IMInterface::getRtsRoomName()
{
	return mRtsRoomName;
}

void IMInterface::setRtsData(const std::string & data)
{
	mRtsData = data;
}

const std::string &IMInterface::getData()
{
	return mRtsData;
}

void IMInterface::setLastError(const QString &error)
{
	mLastError = error;
	emit hasError(error);
}

const QString &IMInterface::getLastError()
{
	return mLastError;
}

void IMInterface::initVChatCallback()
{
	VChat::NetDetect(&CallbackNetDetect);
	//注册音视频回调
	VChat::SetVideoDataCb(true, &CallbackVideoCaptureData);
	VChat::SetVideoDataCb(false, &CallbackVideoRecData);
	VChat::SetAudioDataCb(true, &CallbackAudioCaptureData);
	VChat::SetAudioDataCb(false, &CallbackAudioRecData);
	VChat::SetCbFunc(&CallbackVChatCb);
}

void IMInterface::createVChatRoom(const std::string &name, const std::string &custom_info /* = "" */)
{
	mVChatRoomName = name;
	VChat::CreateRoom(mVChatRoomName, custom_info, "", &CallbackOptCall);
}

void IMInterface::joinVChatRoom(int chatMode, const std::string &name, const std::string &json_extension /* = "" */)
{
	switch (chatMode)
	{
	case 1:
		VChat::JoinRoom(kNIMVideoChatModeAudio, name, json_extension, &CallbackOpt2Call);
		break;
	case 2:
		VChat::JoinRoom(kNIMVideoChatModeVideo, name, json_extension, &CallbackOpt2Call);
		break;
	default:
		break;
	}
}

void IMInterface::setVChatSessionID(const std::string &sessionID)
{
	mVChatSession_id = sessionID;
}

const std::string &IMInterface::getVChatSessionID()
{
	return mVChatSession_id;
}

void IMInterface::setVChatRoomName(const std::string &roomName)
{
	mVChatRoomName = roomName;
}

const std::string &IMInterface::getVChatRoomName()
{
	return mVChatRoomName;
}

void IMInterface::EnumDeviceDevpath(int deviceType)
{
	switch (deviceType)
	{
	case 0:
		VChat::EnumDeviceDevpath(kNIMDeviceTypeAudioIn, &CallbackDeviceDevpath);
		break;
	case 3:
		VChat::EnumDeviceDevpath(kNIMDeviceTypeVideo, &CallbackDeviceDevpath);
		break;
	default:
		break;
	}
}

void IMInterface::startDevice(int type, const std::string& device_path, unsigned fps, int width, int height)
{
	switch (type)
	{
	case 0:
		VChat::StartDevice(kNIMDeviceTypeAudioIn, device_path, fps, width, height, &CallbackStartDevice);
		break;
	case 3:
		VChat::StartDevice(kNIMDeviceTypeVideo, device_path, fps, width, height, &CallbackStartDevice);
		break;
	default:
		break;
	}
}

void IMInterface::endDevice(int type)
{
	switch (type)
	{
	case 0:
		VChat::EndDevice(kNIMDeviceTypeAudioIn);
		break;
	case 3:
		VChat::EndDevice(kNIMDeviceTypeVideo);
		break;
	default:
		break;
	}
}

void IMInterface::addDeviceInfo(const DevInfo &dInfo)
{
	mDeviceInfoMap.insert(dInfo.name, dInfo);
}

void IMInterface::addDeviceInfo(int type, const char *json_string)
{
	QJsonDocument document = QJsonDocument::fromJson(json_string);
	QJsonArray deviceArray = document.array();
	foreach (const QJsonValue &value, deviceArray)
	{
		QJsonObject obj = value.toObject();
		DevInfo info;
		info.type = type;
		info.name = obj["name"].toString();
		info.path = obj["path"].toString();
		mDeviceInfoMap.insert(info.name, info);
	}
}

const DeviceInfoMap &IMInterface::getDeviceInfos()
{
	return mDeviceInfoMap;
}

void CallbackStartChannel(nim::NIMResCode res_code, const std::string& session_id, int channel_type, const std::string& uid)
{
	if (nim::kNIMResSuccess == res_code)
	{
		IMInterface::getInstance()->setRtsSessionID(session_id);
	}
}

void CallbackStartNotify(const std::string& session_id, int channel_type, const std::string& uid, const std::string& custom_info)
{
	if (IMInterface::getInstance()->getRtsSessionID().empty())
	{
		IMInterface::getInstance()->setRtsSessionID(session_id);
		Rts::Ack(session_id, channel_type, true, false, false, &CallbackAck);
	}
	else
	{
		Rts::Ack(session_id, channel_type, false, false, false, &CallbackAck);
	}
}

void CallbackConnectNotify(const std::string& session_id, int channel_type, int code, const std::string& json)
{
	if (session_id == IMInterface::getInstance()->getRtsSessionID())
	{
		if (code != 200)//连接异常，挂断
		{
			Rts::Hangup(session_id.c_str(), &CallbackHangup);
			IMInterface::getInstance()->setRtsSessionID("");
		}
	}
}

void CallbackHangup(nim::NIMResCode res_code, const std::string& session_id)
{
}

void CallbackCreateConf(nim::NIMResCode res_code)
{
	IMInterface *instance = IMInterface::getInstance();
	if (NULL == instance)
	{
		return;
	}

	switch (res_code)
	{
	case nim::kNIMResSuccess://房间创建成功
		emit instance->createRtsRoomSuccessfully(instance->getRtsRoomName());
		break;
	case nim::kNIMResExist://房间已存在
		emit instance->rtsRoomExistence(instance->getRtsRoomName());
		break;
	case nim::kNIMResForbidden://没有权限，禁止操作
		instance->setLastError(ERROR_INFO(res_code, "没有权限，禁止操作"));
		break;
	default:
		break;
	}
	instance = NULL;
}

void CallbackJoinConf(nim::NIMResCode res_code, const std::string& session_id, __int64 channel_id, const std::string& custom_info)
{
	if (nim::kNIMResSuccess == res_code)
	{
		IMInterface::getInstance()->setRtsSessionID(session_id);
		emit IMInterface::getInstance()->joinRtsRoomSuccessfully(session_id, channel_id, custom_info);
	}
}

void CallbackAck(nim::NIMResCode res_code, const std::string& session_id, int channel_type, bool accept)
{

}

void CallbackAckNotify(const std::string& session_id, int channel_type, bool accept, const std::string& uid)
{
	if (session_id == IMInterface::getInstance()->getRtsSessionID())
	{
		if (accept)
		{
			//对方同意，sdk底层开始连接
		}
		else
		{
			IMInterface::getInstance()->setRtsSessionID("");
		}
	}
}

void CallbackSyncAckNotify(const std::string& session_id, int channel_type, bool accept)
{

}

void CallbackMemberNotify(const std::string& session_id, int channel_type, const std::string& uid, int code)
{
	if (session_id == IMInterface::getInstance()->getRtsSessionID())
	{
		if (channel_type == kNIMRtsMemberStatusJoined)
		{
			//成员进入，此时可以在tcp通道发送数据
		}
	}
}

void CallbackHangupNotify(const std::string& session_id, const std::string& uid)
{
	if (session_id == IMInterface::getInstance()->getRtsSessionID())
	{
		IMInterface::getInstance()->setRtsSessionID("");
	}
}

void CallbackControl(nim::NIMResCode res_code, const std::string& session_id, const std::string& info)
{

}

void CallbackControlNotify(const std::string& session_id, const std::string& info, const std::string& uid)
{

}

void CallbackRecData(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data)
{
	if (session_id == IMInterface::getInstance()->getRtsSessionID())
	{
		emit IMInterface::getInstance()->rtsDataReceived(data);
	}	
}

void CallbackNetDetect(int code, nim::NetDetectCbInfo info)
{
	if (kNIMResSuccess == code)
	{

	}
}

void CallbackOptCall(int code, __int64 channel_id, const std::string& json_extension)
{
	IMInterface *instance = IMInterface::getInstance();
	if (NULL == instance)
	{
		return;
	}

	switch (code)
	{
	case kNIMResSuccess:
		emit instance->createVChatRoomSuccessfully(instance->getVChatRoomName());
		break;
	case kNIMResExist:
		emit instance->vChatRoomExistence(instance->getVChatRoomName());
		break;
	case kNIMResForbidden://没有权限，禁止操作
		instance->setLastError(ERROR_INFO(code, "没有权限，禁止操作"));
		break;
	default:
		break;
	}

	instance = NULL;
}

void CallbackOpt2Call(int code, __int64 channel_id, const std::string& json_extension)
{
	IMInterface *instance = IMInterface::getInstance();
	if (NULL == instance)
	{
		return;
	}

	switch (code)
	{
	case kNIMResSuccess:
		emit instance->joinVChatRoomSuccessfully(instance->getVChatRoomName(), channel_id, json_extension);
		break;
	case kNIMResForbidden://没有权限，禁止操作
		instance->setLastError(ERROR_INFO(code, "没有权限，禁止操作"));
		break;
	default:
		break;
	}

	instance = NULL;
}

void CallbackVideoCaptureData(uint64_t time, const char *data, unsigned int size, unsigned int width, unsigned int height, const char *json_extension, const void *user_data)
{
	//char *buf = new char[size + 1];
	//memset(buf, 0, size + 1);
	//memcpy(buf, data, size);
	//delete[]buf;
}

void CallbackVideoRecData(uint64_t time, const char *data, unsigned int size, unsigned int width, unsigned int height, const char *json_extension, const void *user_data)
{

}

void CallbackAudioCaptureData(uint64_t time, const char *data, unsigned int size, const char *json_extension, const void *user_data)
{

}

void CallbackAudioRecData(uint64_t time, const char *data, unsigned int size, const char *json_extension, const void *user_data)
{

}

void CallbackVChatCb(nim::NIMVideoChatSessionType type, __int64 channel_id, int code, const char *json, const void*)
{

}

void CallbackDeviceDevpath(bool ret, nim::NIMDeviceType type, const char *json_extension, const void *user_data)
{
	if (!ret)
	{
		return;
	}

	IMInterface::getInstance()->addDeviceInfo(type, json_extension);
	switch (type)
	{
	case nim::kNIMDeviceTypeAudioIn:
		qDebug() << __FILE__ << __LINE__ << json_extension;
		break;
	case nim::kNIMDeviceTypeAudioOut:
		break;
	case nim::kNIMDeviceTypeAudioOutChat:
		break;
	case nim::kNIMDeviceTypeVideo:
		qDebug() << __FILE__ << __LINE__ << json_extension;
		break;
	case nim::kNIMDeviceTypeSoundcardCapturer:
		break;
	case nim::kNIMDeviceTypeAudioHook:
		break;
	default:
		break;
	}

	emit IMInterface::getInstance()->deviceInfos(type);
}

void CallbackStartDevice(nim::NIMDeviceType type, bool ret, const char *json_extension, const void *user_data)
{
	if (ret)
	{
		qDebug() << VChat::Start(kNIMVideoChatModeVideo, "", "", "");
		emit IMInterface::getInstance()->startDeviceSuccessfully(type);
	}
}