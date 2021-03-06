﻿#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#include <QObject>
#include <QMap>

#include "nim_cpp_rts.h"
#include "nim_cpp_vchat.h"
#include "nim_cpp_doc_trans.h"
#include "nim_cpp_nos.h"

/****************************************************************白板会话相关回调***********************************************************************************/
void CallbackStartChannel(nim::NIMResCode res_code, const std::string& session_id, int channel_type, const std::string& uid);
void CallbackStartNotify(const std::string& session_id, int channel_type, const std::string& uid, const std::string& custom_info);
void CallbackCreateConf(nim::NIMResCode res_code);
void CallbackJoinConf(nim::NIMResCode res_code, const std::string& session_id, __int64 channel_id, const std::string& custom_info);
void CallbackAck(nim::NIMResCode res_code, const std::string& session_id, int channel_type, bool accept);
void CallbackAckNotify(const std::string& session_id, int channel_type, bool accept, const std::string& uid);
void CallbackSyncAckNotify(const std::string& session_id, int channel_type, bool accept);
void CallbackConnectNotify(const std::string& session_id, int channel_type, int code, const std::string& json);
void CallbackMemberNotify(const std::string& session_id, int channel_type, const std::string& uid, int code);
void CallbackHangup(nim::NIMResCode res_code, const std::string& session_id);
void CallbackHangupNotify(const std::string& session_id, const std::string& uid);
void CallbackControl(nim::NIMResCode res_code, const std::string& session_id, const std::string& info);
void CallbackControlNotify(const std::string& session_id, const std::string& info, const std::string& uid);
void CallbackRecData(const std::string& session_id, int channel_type, const std::string& uid, const std::string& data);

/****************************************************************音视频相关回调***********************************************************************************/
void CallbackNetDetect(int code, nim::NetDetectCbInfo info);								/**< 网络探测回调模板 */
void CallbackMp4Opt(bool ret, int code, const std::string& file, __int64 time);	   			/**< MP4录制事件通知回调模板 */
void CallbackAudioRecord(bool ret, int code, const std::string& file, __int64 time);		/**< 音频录制事件通知回调模板 */
void CallbackOptCall(int code, __int64 channel_id, const std::string& json_extension);		/**< 操作回调模板 创建房间回调*/
void CallbackOpt2Call(int code, __int64 channel_id, const std::string& json_extension);	    /**< 操作回调模板 加入房间回调*/	

//采集的视频数据
void CallbackVideoCaptureData(uint64_t time, const char *data, unsigned int size, unsigned int width, unsigned int height, const char *json_extension, const void *user_data);
//接收的视频数据
void CallbackVideoRecData(uint64_t time, const char *data, unsigned int size, unsigned int width, unsigned int height, const char *json_extension, const void *user_data);
//采集的音频数据
void CallbackAudioCaptureData(uint64_t time, const char *data, unsigned int size, const char *json_extension, const void *user_data);
//接收的音频数据
void CallbackAudioRecData(uint64_t time, const char *data, unsigned int size, const char *json_extension, const void *user_data);
//音视频通话中状态返回的回调
void CallbackVChatCb(nim::NIMVideoChatSessionType type, __int64 channel_id, int code, const char *json, const void*);

/****************************************************************音视频设备回调***********************************************************************************/
void CallbackDeviceDevpath(bool ret, nim::NIMDeviceType type, const char *json_extension, const void *user_data);//遍历设备回调
void CallbackStartDevice(nim::NIMDeviceType type, bool ret, const char *json_extension, const void *user_data);//启动设备异步回调

/****************************************************************文件传输相关回调***********************************************************************************/
void CallbackDownloadMedia(nim::NIMResCode res_code, const std::string& file_path, const std::string& call_id, const std::string& res_id); /**< 下载回调模板 */
void CallbackUploadMedia(nim::NIMResCode res_code, const std::string& url); /**< 上传回调模板 */
void CallbackProgress(int64_t completed_size, int64_t file_size); /**< 过程回调模板 */

void CallbackDownloadMediaEx(nim::NIMResCode res_code, const nim::DownloadMediaResult& result); /**< 扩展下载回调模板 */
void CallbackUploadMediaEx(nim::NIMResCode res_code, const nim::UploadMediaResult& result); /**< 扩展上传回调模板 */
void CallbackProgressEx(int64_t completed_size, int64_t file_size, const nim::ProgressData& result); /**< 扩展过程回调模板 */

void CallbackSpeed(int64_t speed); /**< 速度回调模板 */
void CallbackTransferInfo(int64_t actual_size, int64_t speed); /**< 最终传输信息回调模板 */

void CallbackDocInfo(int32_t code, const nim::DocTransInfo& doc_info);

typedef struct DeviceInfo
{
	int type;
	QString name;
	QString path;
}DevInfo;

typedef QMap< QString, DevInfo > DeviceInfoMap;

class IMInterface : public QObject
{
	Q_OBJECT

public:
	~IMInterface();

	static IMInterface *getInstance();
	static void destroy();

	/****************************************************************白板相关***********************************************************************************/
	void initWhiteBoard();								//初始化白板
	void destroyRts();									//清理白板

	void initRtsCallback();								//初始化白板相关回调

	//void createRtsRoom(int channel_type, const QString& uid, const QString& apns, const QString& custom_info, bool data_record, bool audio_record);  //创建一对一白板直播房间
	void createRtsRoom(const std::string &name, const std::string &custom_info = "");			//创建多人白板直播房间
	void joinRtsRoom(const std::string &name, bool record = false);									//加入多人白板直播房间  record 服务器是否记录白板数据
	void SendData(const std::string& session_id, int channel_type, const std::string& data, const std::string& uid = "");		//发送白板数据

	void setRtsSessionID(const std::string &sessionID);	//设置会话ID
	const std::string &getRtsSessionID();				//获取会话ID

	void setRtsRoomName(const std::string &roomName);	//设置白板会话房间名
	const std::string &getRtsRoomName();				//获取白板会话房间名

	void setRtsData(const std::string & data);			//设置接收到的白板数据
	const std::string &getData();						//获取接收到的白板数据

	/****************************************************************音视频相关***********************************************************************************/
	void initVChat();									//初始化音视频
	void destroyVChat();								//清理音视频

	void initVChatCallback();							//初始化音视频相关回调

	void createVChatRoom(const std::string &name, const std::string &custom_info = "");								//创建多人音视频房间
	//json_extension 可选 如{"custom_video":0, "custom_audio":0, "video_quality":0, "session_id":"1231sda"}
	void joinVChatRoom(int chatMode, const std::string &name, const std::string &json_extension = "");				//加入多人音视频房间  chatMode 通话模式：1为语音，2为视频

	void setVChatSessionID(const std::string &sessionID);	//设置音视频会话ID
	const std::string &getVChatSessionID();					//获取音视频会话ID

	void setVChatRoomName(const std::string &roomName);		//设置音视频会话房间名
	const std::string &getVChatRoomName();					//获取音视频会话房间名

	/****************************************************************音视频设备相关***********************************************************************************/
	void EnumDeviceDevpath(int deviceType);				//遍历设备	deviceType  设备类型   0 麦克风  3 摄像头
	void startDevice(int type, const std::string& device_path, unsigned fps, int width, int height);		//启动设备
	void endDevice(int type);																				//关闭设备

	void addDeviceInfo(const DevInfo &dInfo);					//添加一个设备信息
	void addDeviceInfo(int type, const char *json_string);		//根据json字符串解析设备信息并添加至设备列表
	const DeviceInfoMap &getDeviceInfos();						//获取设备列表

	void setLastError(const QString &error);					//设置最后一次错误信息
	const QString &getLastError();								//获取最后一次错误信息

	/****************************************************************文件传输相关***********************************************************************************/
	void initFileTransferCallback();							//初始化文档转换回调函数
	void uploadFile(const std::string& local_file, const std::string& json_extension);		//上传文件  参数local_file为文件绝对路径，json_extension为扩展参数
	void downloadFile(const std::string& nos_url, const std::string& json_extension);		//下载文件  参数nos_url为云端url，json_extension为扩展参数
	/** @fn std::string createJsonExtension(const std::string& name, int source_type, int pic_type, int upload_type = 1, const std::string& doc_trans_ext = "");
	* 生成上传转码文件需要的json_extension 
	* @param[in] name 文档名称
	* @param[in] source_type 转码源文档的文件类型	1:ppt  2:pptx  3:pdf 
	* @param[in] pic_type 转码目标图片的文件类型	10:jpg	11:png
	* @param[in] upload_type HTTP上传任务的类型		0:普通文件上传 1:文档转换上传  默认值为1
	* @param[in] doc_trans_ext HTTP上传任务的类型	文档转换时的扩展参数，在成功后能查询到  默认值为""
	* @return std::string json_extension字符串
	*/
	std::string createJsonExtension(const std::string& name, int source_type, int pic_type, int upload_type = 1, const std::string& doc_trans_ext = "");
	std::string getPageUrl(int32_t page_num);		//根据页码获取转换完成后图片的下载地址

	void setDocTransInfo(const nim::DocTransInfo& docInfo);		//设置转换文档信息
	const nim::DocTransInfo& getDocTransInfo();					//获取转换文档信息

signals:
	/****************************************************************白板相关***********************************************************************************/
	void createRtsRoomSuccessfully(const std::string&);
	void rtsRoomExistence(const std::string&);
	void joinRtsRoomSuccessfully(const std::string&, __int64, const std::string&);
	void rtsDataReceived(const std::string &);

	/****************************************************************音视频相关***********************************************************************************/
	void createVChatRoomSuccessfully(const std::string&);
	void vChatRoomExistence(const std::string&);
	void joinVChatRoomSuccessfully(const std::string&, __int64, const std::string&);

	/****************************************************************音视频设备相关***********************************************************************************/
	void deviceInfos(int);											//设备已遍历完成   参数为设备类型
	void startDeviceSuccessfully(int);								//设备启动成功     参数为设备类型

	/****************************************************************文件传输相关***********************************************************************************/
	void transferProgress(int64_t, int64_t);						//参数1  已完成数据大小		参数2  文件总大小
	void transferSpeed(int64_t);									//文件传输速度
	void transferInfo(int64_t, int64_t);							//参数1  传输数据实际大小		参数2  平均速度

	void hasError(const QString &);

private:
	IMInterface(QObject *parent = 0);

private:
	static IMInterface *mInstace;

	/****************************************************************白板相关***********************************************************************************/
	std::string mRtsSession_id;		//白板会话ID
	std::string mRtsRoomName;		//白板房间名
	std::string mRtsData;			//白板数据

	bool mRtsRoomCreated;	//是否已创建RtsRoom
	bool mRtsRoomJoined;	//是否已加入RtsRoom
	bool mRtsRecord;		//是否记录白板数据

	/****************************************************************音视频相关***********************************************************************************/
	bool mVChatInitSuccess; //音视频是否初始化成功

	std::string mVChatSession_id;	//音视频会话ID
	std::string mVChatRoomName;		//音视频房间名

	QString mLastError;

	DeviceInfoMap mDeviceInfoMap;	//设备类别

	nim::DocTransInfo mDocTransInfo;		//转换完成的文档信息
};

#endif // WHITEBOARD_H
