﻿/** @file nim_cpp_global.cpp
  * @brief NIM SDK提供的一些全局接口
  * @copyright (c) 2015-2017, NetEase Inc. All rights reserved
  * @author towik, Oleg, Harrison
  * @date 2015/2/1
  */

#include "nim_cpp_global.h"
#include "nim_sdk_util.h"
#include "nim_client_def.h"
#include "nim_string_util.h"

namespace nim
{
#ifdef NIM_SDK_DLL_IMPORT
typedef	void (*nim_global_free_str_buf)(char *str);
typedef	void (*nim_global_free_buf)(void *data);
typedef void (*nim_global_set_proxy)(NIMProxyType, const char*, int, const char*, const char*);
typedef void(*nim_global_reg_sdk_log_cb)(const char *json_extension, nim_sdk_log_cb_func cb, const void *user_data);
#if NIMAPI_UNDER_WIN_DESKTOP_ONLY
typedef void (*nim_global_detect_proxy)(enum NIMProxyType type, const char *host, int port, const char *user, const char *password, nim_global_detect_proxy_cb_func cb, const void *user_data);
#endif
#else
#include "nim_global.h"
#endif


void Global::FreeStrBuf(char *str)
{
	return NIM_SDK_GET_FUNC(nim_global_free_str_buf)(str);
}

void Global::FreeBuf(void *data)
{
	return NIM_SDK_GET_FUNC(nim_global_free_buf)(data);
}

void Global::SetProxy(NIMProxyType type, const std::string& host, int port, const std::string& user, const std::string& password)
{
	return NIM_SDK_GET_FUNC(nim_global_set_proxy)(type, host.c_str(), port, user.c_str(), password.c_str());
}

#if NIMAPI_UNDER_WIN_DESKTOP_ONLY
static void CallbackDetectProxy(bool connect, NIMProxyDetectStep step, const char *json_extention, const void *user_data)
{
	if (user_data)
	{
		Global::DetectProxyCallback* cb = (Global::DetectProxyCallback*)user_data;
		if (cb != nullptr)
		{
			(*cb)(connect, step, PCharToString(json_extention));
		}
		delete cb;
	}
}

void Global::DetectProxy(NIMProxyType type, const std::string& host, int port, const std::string& user, const std::string& password, const Global::DetectProxyCallback& callback)
{
	Global::DetectProxyCallback *cb_pointer = nullptr;
	if (callback)
	{
		cb_pointer = new Global::DetectProxyCallback(callback);
	}
	NIM_SDK_GET_FUNC(nim_global_detect_proxy)(type, host.c_str(), port, user.c_str(), password.c_str(), &CallbackDetectProxy, cb_pointer);
}
#endif

static void CallbackSDKLog(int log_level, const char *log, const void *user_data)
{
	Global::SDKLogCallback* cb = (Global::SDKLogCallback*)user_data;
	if (cb != nullptr)
	{
		std::string a;
		a.append(log);
		(*cb)(log_level, a);
	}
	//delete cb;
}

void Global::SetSDKLogCallback(const std::string&json_extension, const SDKLogCallback& callback)
{
	SDKLogCallback* cb_pointer = nullptr;
	if (callback)
	{
		cb_pointer = new SDKLogCallback(callback);
	}
	NIM_SDK_GET_FUNC(nim_global_reg_sdk_log_cb)(json_extension.c_str(), &CallbackSDKLog, cb_pointer);
}
}