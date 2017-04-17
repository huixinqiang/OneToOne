#include "UIWindowSet_oneToOne.h"
#include <QPainter>
#include <windowsx.h>
#include "wtypes.h"
#include <QMouseEvent>
#include <QVideoWidget>
#include <QToolTip>
#include <iosfwd>
#include <sstream>
#include <tlhelp32.h>

#include "palette.h"
#include "colorpicker.h"
#include "IMInterface.h"

#include <QColorDialog>
#include <QMenu>
#include <QDebug>

#define MAINWINDOW_X_MARGIN 10
#define MAINWINDOW_Y_MARGIN 10
#define MAINWINDOW_TITLE_HEIGHT 0

#define Audio 0	//音频设备
#define Video 3	//视频设备
#define AudioMode 1	//语音通话
#define VideoMode 2 //视频通话

#ifdef TEST
#define _DEBUG
#else
#endif

UIWindowSet_oneToOne::UIWindowSet_oneToOne(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);

	mColorPicker = NULL;
	mWhiteBoard = NULL;
	mRtsRecord = false;
	mVChatMode = VideoMode;		//默认启动视频会话

	IMInterface::getInstance()->initVChat();

	initWhiteBoardWidget();
	initColorPicker();
	initConnection();
	initDeviceMenu();
	initVideoInfo();
}

UIWindowSet_oneToOne::UIWindowSet_oneToOne(const QString &roomName, const QString &roomInfo/* = ""*/, QWidget *parent/* = 0*/)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);

	mColorPicker = NULL;
	mWhiteBoard = NULL;
	mRtsRecord = false;
	mVChatMode = VideoMode;		//默认启动视频会话

	IMInterface::getInstance()->initVChat();

	initWhiteBoardWidget();
	initColorPicker();
	initConnection();
	initDeviceMenu();
	initVideoInfo();
	createRtsRoom(roomName, roomInfo);
}

UIWindowSet_oneToOne::~UIWindowSet_oneToOne()
{
	if (mColorPicker)
	{
		delete mColorPicker;
	}

	IMInterface::destroy();
}

void UIWindowSet_oneToOne::createRtsRoom(const QString &roomName, const QString &roomInfo /* = "" */)
{
	IMInterface::getInstance()->createRtsRoom(roomName.toStdString(), roomInfo.toStdString());
	IMInterface::getInstance()->createVChatRoom(roomName.toStdString(), roomInfo.toStdString());
	IMInterface::getInstance()->EnumDeviceDevpath(Audio);
	IMInterface::getInstance()->EnumDeviceDevpath(Video);
}

void UIWindowSet_oneToOne::initWhiteBoardWidget()
{
	if (mWhiteBoard)
	{
		delete mWhiteBoard;
	}

	mWhiteBoard = new Palette();
	ui.horizontalLayout_whiteBoard->addWidget(mWhiteBoard);
	mWhiteBoard->setIsDraw(ui.pushButton_pen->isChecked());
}

void UIWindowSet_oneToOne::initColorPicker()
{
	if (mColorPicker)
	{
		delete mColorPicker;
	}
	mColorPicker = new ColorPicker();

	if (mWhiteBoard)
	{
		QColor color = mWhiteBoard->getPenColor();
		mColorPicker->setCurColor(color);
	}

	connect(mColorPicker, SIGNAL(selectColor(QColor)), this, SLOT(setPenColor(QColor)));
}

void UIWindowSet_oneToOne::initConnection()
{
	IMInterface *instance = IMInterface::getInstance();
	if (NULL == instance)
	{
		return;
	}

	connect(instance, SIGNAL(createRtsRoomSuccessfully(const std::string&)), this, SLOT(joinRtsRoom(const std::string &)));
	connect(instance, SIGNAL(rtsRoomExistence(const std::string&)), this, SLOT(joinRtsRoom(const std::string &)));
	connect(instance, SIGNAL(joinRtsRoomSuccessfully(const std::string&, __int64, const std::string&)), 
		this, SLOT(joinRtsRoomSuccessfully(const std::string&, __int64, const std::string&)));

	connect(instance, SIGNAL(createVChatRoomSuccessfully(const std::string&)), this, SLOT(joinVChatRoom(const std::string &)));
	connect(instance, SIGNAL(vChatRoomExistence(const std::string&)), this, SLOT(joinVChatRoom(const std::string &)));
	connect(instance, SIGNAL(joinVChatRoomSuccessfully(const std::string&, __int64, const std::string&)),
		this, SLOT(joinVChatRoomSuccessfully(const std::string&, __int64, const std::string&)));

	connect(instance, SIGNAL(hasError(const QString &)), this, SLOT(errorInfo(const QString &)));
	connect(instance, SIGNAL(deviceInfos(int)), this, SLOT(setDeviceInfos(int)));
	connect(instance, SIGNAL(startDeviceSuccessfully(int)), this, SLOT(startDeviceSuccessfully(int)));

	connect(&mAudioMenu, SIGNAL(triggered(QAction *)), this, SLOT(startDevice(QAction *)));
	connect(&mVideoMenu, SIGNAL(triggered(QAction *)), this, SLOT(startDevice(QAction *)));
}

void UIWindowSet_oneToOne::initDeviceMenu()
{
	ui.toolButton_audio->setMenu(&mAudioMenu);
	ui.toolButton_video->setMenu(&mVideoMenu);
}

void UIWindowSet_oneToOne::initVideoInfo()
{
	//后期根据需求设置
	mVideoFps = 20;
	mVideoWidth = 100;
	mVideoHeight = 100;
}

void UIWindowSet_oneToOne::on_min_pushButton_clicked()
{
	showMinimized();
}

void UIWindowSet_oneToOne::on_max_pushButton_clicked()
{
	if (isMaximized())
	{
		showNormal();
	}
	else
	{
		showMaximized();
	}	
}

void UIWindowSet_oneToOne::on_close_pushButton_clicked()
{
	close();
}

void UIWindowSet_oneToOne::on_pushButton_revocation_clicked()
{
	mWhiteBoard->revocation();
}

void UIWindowSet_oneToOne::on_pushButton_selectColor_clicked()
{
	if (mColorPicker)
	{
		mColorPicker->show();

		QPoint point = QCursor::pos();
		int height = mColorPicker->height();
		mColorPicker->move(point.x(), point.y() - height);
	}
}

void UIWindowSet_oneToOne::on_pushButton_pen_clicked(bool checked)
{
	mWhiteBoard->setIsDraw(checked);
	if (checked)
	{
		QString styleSheet = "background-color: rgb(165, 165, 165);";
		ui.pushButton_pen->setStyleSheet(styleSheet);
	}
	else
	{
		ui.pushButton_pen->setStyleSheet("");
	}
}

void UIWindowSet_oneToOne::on_toolButton_audio_clicked(bool checked)
{
	if (checked)
	{
		IMInterface::getInstance()->endDevice(Audio);
	}
	else
	{
		QString dPath = IMInterface::getInstance()->getDeviceInfos().value(mCurAudioDeviceName).path;
		IMInterface::getInstance()->startDevice(Audio, dPath.toStdString(), 0, 0, 0);
	}
}

void UIWindowSet_oneToOne::on_toolButton_video_clicked(bool checked)
{
	if (checked)
	{
		IMInterface::getInstance()->endDevice(Audio);
	}
	else
	{
		QString dPath = IMInterface::getInstance()->getDeviceInfos().value(mCurAudioDeviceName).path;
		IMInterface::getInstance()->startDevice(Video, dPath.toStdString(), mVideoFps, mVideoWidth, mVideoHeight);
	}
}

void UIWindowSet_oneToOne::on_toolButton_volume_clicked(bool checked)
{

}

void UIWindowSet_oneToOne::setPenColor(const QColor &color)
{
	if (mWhiteBoard)
	{
		QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue());
		ui.pushButton_selectColor->setStyleSheet(styleSheet);
		mWhiteBoard->setPenColor(color);
	}
}

void UIWindowSet_oneToOne::joinRtsRoom(const std::string &roomName)
{
	IMInterface::getInstance()->joinRtsRoom(roomName, mRtsRecord);
}

void UIWindowSet_oneToOne::joinVChatRoom(const std::string &roomName)
{
	IMInterface::getInstance()->joinVChatRoom(mVChatMode,  roomName, mVChatExtension);
}

void UIWindowSet_oneToOne::joinRtsRoomSuccessfully(const std::string &session_id, __int64 channel_id, const std::string &custom_info)
{
	QString info = QString("%1 %2 session_id=%3 channel_id=%4 custom_info=%5")
		.arg(__FILE__).arg(__LINE__).arg(session_id.c_str()).arg(channel_id).arg(custom_info.c_str());
	ui.textBrowser->append(info);
}

void UIWindowSet_oneToOne::joinVChatRoomSuccessfully(const std::string &roomName, __int64 channel_id, const std::string&json_extension)
{
	QString info = QString("%1 %2 roomName=%3 channel_id=%4 json_extension=%5")
		.arg(__FILE__).arg(__LINE__).arg(roomName.c_str()).arg(channel_id).arg(json_extension.c_str());
	ui.textBrowser->append(info);
}

void UIWindowSet_oneToOne::errorInfo(const QString &error)
{
	ui.textBrowser->append(error);
}

void UIWindowSet_oneToOne::setDeviceInfos(int type)
{
	switch (type)
	{
	case Audio:
	{
		mAudioMenu.clear();
		foreach(const DevInfo &info, IMInterface::getInstance()->getDeviceInfos())
		{
			if (info.type == type)
			{
				QAction *action = mAudioMenu.addAction(info.name);
				action->setCheckable(true);
				mActionMap.insert(info.name, action);
			}
		}
	}		
		break;
	case Video:
		mVideoMenu.clear();
		foreach(const DevInfo &info, IMInterface::getInstance()->getDeviceInfos())
		{
			if (info.type == type)
			{
				QAction *action = mVideoMenu.addAction(info.name);
				action->setCheckable(true);
				mActionMap.insert(info.name, action);
			}			
		}
		break;
	default:
		break;
	}
	
}

void UIWindowSet_oneToOne::startDevice(QAction *action)
{
	QString deviceName = action->text();

	const DeviceInfoMap &dMap = IMInterface::getInstance()->getDeviceInfos();
	const DevInfo &dInfo = dMap.value(deviceName);
	switch (dInfo.type)
	{
	case Audio:
	{
		QList< QAction * > actions = mAudioMenu.actions();
		foreach(QAction *action, actions)
		{
			action->setChecked(false);
		}
	}		
	IMInterface::getInstance()->startDevice(dInfo.type, dInfo.name.toStdString(), 0, 0, 0);
		break;
	case Video:
	{
		QList< QAction * > actions = mVideoMenu.actions();
		foreach(QAction *action, actions)
		{
			action->setChecked(false);
		}
	}
	IMInterface::getInstance()->startDevice(dInfo.type, dInfo.name.toStdString(), mVideoFps, mVideoWidth, mVideoHeight);
		break;
	default:
		break;
	}
	action->setChecked(true);	
}

void UIWindowSet_oneToOne::startDeviceSuccessfully(int type)
{
	switch (type)
	{
	case Audio:
		qDebug() << __FILE__ << __LINE__ << "audio started";
		break;
	case Video:
		qDebug() << __FILE__ << __LINE__ << "video started";
		break;
	default:
		break;
	}
}

void UIWindowSet_oneToOne::mousePressEvent(QMouseEvent *event)
{
	if (mColorPicker && mColorPicker->isVisible())
	{
		QPoint point = event->pos();
		QRect rect = mColorPicker->rect();
		if (!rect.contains(point))
		{
			mColorPicker->hide();
		}
	}	
}