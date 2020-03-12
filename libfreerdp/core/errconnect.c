/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Error Connect
 *
 * Copyright 2015 Armin Novak <armin.novak@thincast.com>
 * Copyright 2015 Thincast Technologies GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#include <freerdp/log.h>

#include "errinfo.h"

#define TAG FREERDP_TAG("core")

#define ERRCONNECT_DEFINE(_code)	{ ERRCONNECT_##_code , "ERRCONNECT_" #_code , ERRCONNECT_##_code##_STRING }

/* Protocol-independent codes */

#define ERRCONNECT_PRE_CONNECT_FAILED_STRING \
	"配置错误导致终止连接 A configuration error prevented a connection to be established."

#define ERRCONNECT_CONNECT_UNDEFINED_STRING \
	"发生未定义的连接错误 A undefined connection error occurred."

#define ERRCONNECT_POST_CONNECT_FAILED_STRING \
	"由于连接后配置错误，连接尝试被中止 The connection attempt was aborted due to post connect configuration errors."

#define ERRCONNECT_DNS_ERROR_STRING \
	"DNS无法解析 The DNS entry could not be resolved."

#define ERRCONNECT_DNS_NAME_NOT_FOUND_STRING \
	"未找到DNS主机名称 The DNS host name was not found."

#define ERRCONNECT_CONNECT_FAILED_STRING \
	"连接失败 The connection failed."

#define ERRCONNECT_MCS_CONNECT_INITIAL_ERROR_STRING \
	"初始化MCS连接失败 The connection failed at initial MCS connect"

#define ERRCONNECT_TLS_CONNECT_FAILED_STRING \
	"TLS连接失败 The connection failed at TLS connect."

#define ERRCONNECT_AUTHENTICATION_FAILED_STRING \
	"认证失败终止连接，请检查用户名、密码是否正确 An authentication failure aborted the connection."

#define ERRCONNECT_INSUFFICIENT_PRIVILEGES_STRING \
	"没有足够的权限建立连接 Insufficient privileges to establish a connection."

#define ERRCONNECT_CONNECT_CANCELLED_STRING \
	"连接被取消 The connection was cancelled."

#define ERRCONNECT_SECURITY_NEGO_CONNECT_FAILED_STRING \
	"协商安全设置时失败 The connection failed at negotiating security settings."

#define ERRCONNECT_CONNECT_TRANSPORT_FAILED_STRING \
	"传输层连接失败 The connection transport layer failed."

#define ERRCONNECT_PASSWORD_EXPIRED_STRING \
	"密码已过期，必须修改 The password has expired and must be changed."

#define ERRCONNECT_PASSWORD_CERTAINLY_EXPIRED_STRING \
	"密码已经过期，必须修改 The password has certainly expired and must be changed."

#define ERRCONNECT_CLIENT_REVOKED_STRING \
	"客户端已经被撤销 The client has been revoked."

#define ERRCONNECT_KDC_UNREACHABLE_STRING \
	"KDC无法访问 The KDC is unreachable."

#define ERRCONNECT_ACCOUNT_DISABLED_STRING \
	"该账户已被禁用 The account is disabled."

#define ERRCONNECT_PASSWORD_MUST_CHANGE_STRING \
	"密码必须修改 The password must be changed."

#define ERRCONNECT_LOGON_FAILURE_STRING \
	"登录失败 Logon failed."

#define ERRCONNECT_WRONG_PASSWORD_STRING \
	"提供的密码错误 Wrong password supplied."

#define ERRCONNECT_ACCESS_DENIED_STRING \
	"拒绝访问 Access denied."

#define ERRCONNECT_ACCOUNT_RESTRICTION_STRING \
	"账户限制 Account restriction."

#define ERRCONNECT_ACCOUNT_LOCKED_OUT_STRING \
	"账户被锁定 Account locked out."

#define ERRCONNECT_ACCOUNT_EXPIRED_STRING \
	"账户已过期 Account expired."

#define ERRCONNECT_LOGON_TYPE_NOT_GRANTED_STRING \
	"未授予登录类型 Logon type not granted."

#define ERRCONNECT_NO_OR_MISSING_CREDENTIALS_STRING \
	"凭证无效或丢失 Credentials invalid or missing."

/* Special codes */
#define ERRCONNECT_SUCCESS_STRING "Success."
#define ERRCONNECT_NONE_STRING ""

static const ERRINFO ERRCONNECT_CODES[] =
{
	ERRCONNECT_DEFINE(SUCCESS),

	ERRCONNECT_DEFINE(PRE_CONNECT_FAILED),
	ERRCONNECT_DEFINE(CONNECT_UNDEFINED),
	ERRCONNECT_DEFINE(POST_CONNECT_FAILED),
	ERRCONNECT_DEFINE(DNS_ERROR),
	ERRCONNECT_DEFINE(DNS_NAME_NOT_FOUND),
	ERRCONNECT_DEFINE(CONNECT_FAILED),
	ERRCONNECT_DEFINE(MCS_CONNECT_INITIAL_ERROR),
	ERRCONNECT_DEFINE(TLS_CONNECT_FAILED),
	ERRCONNECT_DEFINE(AUTHENTICATION_FAILED),
	ERRCONNECT_DEFINE(INSUFFICIENT_PRIVILEGES),
	ERRCONNECT_DEFINE(CONNECT_CANCELLED),
	ERRCONNECT_DEFINE(SECURITY_NEGO_CONNECT_FAILED),
	ERRCONNECT_DEFINE(CONNECT_TRANSPORT_FAILED),
	ERRCONNECT_DEFINE(PASSWORD_EXPIRED),
	ERRCONNECT_DEFINE(PASSWORD_CERTAINLY_EXPIRED),
	ERRCONNECT_DEFINE(CLIENT_REVOKED),
	ERRCONNECT_DEFINE(KDC_UNREACHABLE),
	ERRCONNECT_DEFINE(ACCOUNT_DISABLED),
	ERRCONNECT_DEFINE(PASSWORD_MUST_CHANGE),
	ERRCONNECT_DEFINE(LOGON_FAILURE),
	ERRCONNECT_DEFINE(WRONG_PASSWORD),
	ERRCONNECT_DEFINE(ACCESS_DENIED),
	ERRCONNECT_DEFINE(ACCOUNT_RESTRICTION),
	ERRCONNECT_DEFINE(ACCOUNT_LOCKED_OUT),
	ERRCONNECT_DEFINE(ACCOUNT_EXPIRED),
	ERRCONNECT_DEFINE(LOGON_TYPE_NOT_GRANTED),
	ERRCONNECT_DEFINE(NO_OR_MISSING_CREDENTIALS),

	ERRCONNECT_DEFINE(NONE)
};

const char* freerdp_get_error_connect_string(UINT32 code)
{
	const ERRINFO* errInfo;
	errInfo = &ERRCONNECT_CODES[0];

	while (errInfo->code != ERRCONNECT_NONE)
	{
		if (code == errInfo->code)
		{
			return errInfo->info;
		}

		errInfo++;
	}

	return "ERRCONNECT_UNKNOWN";
}

const char* freerdp_get_error_connect_name(UINT32 code)
{
	const ERRINFO* errInfo;
	errInfo = &ERRCONNECT_CODES[0];

	while (errInfo->code != ERRCONNECT_NONE)
	{
		if (code == errInfo->code)
		{
			return errInfo->name;
		}

		errInfo++;
	}

	return "ERRCONNECT_UNKNOWN";
}

