#ifndef UIWINDOWSET_H
#define UIWINDOWSET_H

#include <QWidget>
#include "ui_UIWindowSet_oneToOne.h"
#include <QTimer>
#include <QMenu>

//---云信
#include "nim_client_def.h"
#include "assert.h"
#include <string>

class Palette;
class ColorPicker;

typedef QMap< QString, QAction * > ActionMap;		//音视频设备菜单actionMap   key为设备name		value为对应的菜单action指针

namespace Ui{
	class UIWindowSet_oneToOne;
}

class UIWindowSet_oneToOne : public QWidget
{
	Q_OBJECT

public:
	UIWindowSet_oneToOne(QWidget *parent = 0);
	UIWindowSet_oneToOne(const QString &roomName, const QString &roomInfo = "", QWidget *parent = 0);
	~UIWindowSet_oneToOne();

	void createRtsRoom(const QString &roomName, const QString &roomInfo = "");

private:
	void initWhiteBoardWidget();		//初始化白板界面
	void initColorPicker();				//初始化颜色选择器
	void initConnection();				//初始化信号槽链接
	void initDeviceMenu();				//初始化音视频设备选择菜单
	void initVideoInfo();				//初始化摄像头参数信息

private slots:
	void on_min_pushButton_clicked();				//最小化按钮槽
	void on_max_pushButton_clicked();				//最大化按钮槽
	void on_close_pushButton_clicked();				//关闭按钮槽
	void on_pushButton_revocation_clicked();		//撤销上一步操作槽
	void on_pushButton_selectColor_clicked();		//颜色选择槽
	void on_pushButton_pen_clicked(bool checked);	//画笔使能槽
	void on_toolButton_audio_clicked(bool checked); //麦克风开关
	void on_toolButton_video_clicked(bool checked); //摄像头开关
	void on_toolButton_volume_clicked(bool checked);//声音开关

	void setPenColor(const QColor &color);			//设置画笔颜色槽   接收颜色选择器信号
	void joinRtsRoom(const std::string &roomName);	//房间创建成功后加入房间	白板
	void joinVChatRoom(const std::string &roomName);//房间创建成功后加入房间	音视频
	void joinRtsRoomSuccessfully(const std::string &session_id, __int64 channel_id, const std::string &custom_info);//加入白板房间成功，设置白板房间信息
	void joinVChatRoomSuccessfully(const std::string &roomName, __int64 channel_id, const std::string&json_extension);	//加入音视频房间成功，设置音视频房间信息
	void errorInfo(const QString &error);			//显示错误信息
	void setDeviceInfos(int type);					//根据设备类型设置音视频设备toolbutton菜单
	void startDevice(QAction *action);				//点击音视频设备菜单启动对应设备槽
	void startDeviceSuccessfully(int type);			//设备启动成功		type为设备类型   0  音频（麦克风）   3  视频（摄像头）

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	Ui::UIWindowSet_oneToOne ui;
	
	Palette *mWhiteBoard;					//画板
	
	ColorPicker *mColorPicker;				//颜色选择器

	bool mRtsRecord;						//服务器是否记录白板数据

	QMenu mAudioMenu;						//音频设备选择菜单
	QMenu mVideoMenu;						//视频设备选择菜单
	ActionMap mActionMap;					//音视频设备actionMap
	QString mCurAudioDeviceName;			//当前音频设备名
	QString mCurVideoDeviceName;			//当前视频设备名
	unsigned int mVideoFps;					//摄像头采样频率
	int mVideoWidth;						//摄像头采集宽度期望值
	int mVideoHeight;						//摄像头采集高度期望值

	int mVChatMode;							//音视频模式		1：音频		2：视频
	std::string mVChatExtension;			//音视频加入房间使用扩展参数   暂时未用
		
};

#endif // UIWINDOWSET_H
