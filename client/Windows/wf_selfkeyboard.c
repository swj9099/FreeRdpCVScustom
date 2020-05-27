/**

 */

#include <winpr/windows.h>
#include <freerdp/log.h>

#include <winpr/wlog.h>

#include <winpr/crt.h>
#include <winpr/credui.h>
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <assert.h>
#include <sys/types.h>
#include <wf_selfkeyboard.h>
#include <io.h>
#include <time.h>
#include <process.h>

#include "wf_event.h"
#define TAG CLIENT_TAG("windows")
#define LOGNAME "\\freerdp"

rdpInput* g_myinput = NULL;
BOOL EXCUCESTARTFLAGE=FALSE;
FILE * f_stderr = NULL;
FILE * f_stdout = NULL;

static void flushlog(void)
{
	fflush(stdout);
	fflush(stderr);
}

int wf_change_console(rdpSettings* settings)
{
	char LogPathName[125] = {0};
	BOOL DebugScreen = FALSE;
	int Pidself = 0;
	DebugScreen = settings->DebugScreen;
	Pidself = _getpid();
	sprintf(LogPathName,"%s%s_%d.log",settings->DrivePosition,LOGNAME,Pidself);
	remove(LogPathName);

	if(DebugScreen)
	{
		if (!AllocConsole())
		return 1;

		freopen("CONOUT$", "a+", stdout);
		freopen("CONOUT$", "a+", stderr);
		WLog_INFO(TAG,  "Debug console created.");
		WLog_INFO(TAG,  "settings->DrivePosition is %s",settings->DrivePosition);
		WLog_INFO(TAG,  "LOGNAME is %s",LOGNAME);
		WLog_INFO(TAG,  "Pidself is %d",Pidself);
		WLog_INFO(TAG,  "LogPathName is %s",LogPathName);
	
	}
	else
	{
		
		f_stdout = freopen(LogPathName, "w", stdout);
		f_stderr = freopen(LogPathName, "w", stderr);
		WLog_INFO(TAG,  "%s console created.\n",(char *)LogPathName);
	}
	return 0;
}

int wf_close_console(rdpSettings* settings)
{

	BOOL DebugScreen = FALSE;
	DebugScreen = settings->DebugScreen;

	if(!settings)
		return -1;


	if(DebugScreen)
		return 0;

	flushlog();
	fclose(stdout);
	fclose(stderr);

	if(!f_stdout)
	{
		fclose(f_stdout);
		f_stdout = NULL;
	};
		

	if(!f_stderr)
	{
		fclose(f_stderr);
		f_stderr = NULL;
	}

	return 0;
}

BOOL pre_check_flag(char* filename)
{
	CONST CHAR* mfilename = filename;
	errno_t err = 0;
	if(!mfilename)
	{
		WLog_ERR(TAG,"mfilename is NULL");
		return FALSE;
	}
		
	
	if ((err = _access_s(mfilename, 0)) == 0)
	{
		WLog_INFO(TAG, "[%s] 标志文件已成功检测\n",mfilename);
		return TRUE;
	}
	else
	{
		//WLog_ERR(TAG, "errno is %d",err);
		return FALSE;
	}

}

// 检测flagPoint（setting->StartFlag或setting->EndFlag）是否为空；
// 检测是否包含路径，若不包含，将其与setting->DrivePosition拼接
char* get_flag(rdpSettings* settings, int index)
{
	char* flagPoint = NULL;
	char* subFlag = NULL;
	char* srcFlagPoint = NULL;

	if (0 == index)
		srcFlagPoint = settings->StartFlag;
	else if (1 == index)
		srcFlagPoint = settings->EndFlag;

	if (NULL == srcFlagPoint)
		return NULL;
	if (settings->DrivePosition)
		subFlag = strstr(srcFlagPoint, settings->DrivePosition);
	else
	{
		WLog_ERR(TAG, "Can not find settings->DrivePosition. ");
		return NULL;
	}

	if (NULL == subFlag) //flagPoint中不包含DrivePosition
	{
		if (NULL == settings->DrivePosition)
		{
			WLog_ERR(TAG, "Can not find the path of startflag and endflag. ");
			return NULL;
		}
		else
		{
			int newsize = strlen(srcFlagPoint) + strlen(settings->DrivePosition) + 2;
			flagPoint = (char *)calloc(newsize, sizeof(char));
			strncpy(flagPoint, settings->DrivePosition, strlen(settings->DrivePosition));
			strncat(flagPoint, "\\", 1);
			strncat(flagPoint, srcFlagPoint, strlen(srcFlagPoint));
			flagPoint[newsize - 1] = '\0';
		}
	}
	return flagPoint;
}
// 检测setting->StartFlag是否为空；是否包含路径，若不包含，将其与setting->DrivePosition拼接
BOOL get_startflag(rdpSettings* settings, char** startflag)
{
	if (NULL == (*startflag = get_flag(settings, 0)))
		return FALSE;
	return TRUE;
}
// 检测setting->EndFlagt是否为空；是否包含路径，若不包含，将其与setting->DrivePosition拼接
BOOL get_endflag(rdpSettings* settings, char** endflag)
{
	if (NULL == (*endflag = get_flag(settings, 1)))
		return FALSE;	
	return TRUE;
}

void SelfSendOnekey(DWORD key1)
{
	DWORD mykey = 0;

	if(NULL == g_myinput)
	{
		WLog_ERR(TAG, "Keyboard_init is faild");
		return;
	}

	switch (key1)
	{
		case KEY_ENTER:
			mykey = RDP_SCANCODE_RETURN;
			break;
		case KEY_ESC:
			mykey = RDP_SCANCODE_ESCAPE;
			break;
		case KEY_WIN:
			mykey = RDP_SCANCODE_LWIN;
			break;
		case KEY_BACK:
			mykey = RDP_SCANCODE_BACKSPACE;
			break;
		case KEY_CTRL:
			mykey = RDP_SCANCODE_LCONTROL;
			break;
		case KEY_SHIFT:
			mykey = RDP_SCANCODE_LSHIFT;
			break;
		case KEY_ALT:
			mykey = RDP_SCANCODE_LMENU;
			break;
		case KEY_LEFT:
			mykey = RDP_SCANCODE_LEFT;
			break;
		default:
			WLog_ERR(TAG, "undefined Onekey!!!");

	}	

	freerdp_input_send_keyboard_event_ex(g_myinput, TRUE, mykey);
	freerdp_input_send_keyboard_event_ex(g_myinput, FALSE, mykey);
	return;
}

void SelfSendDoublekey(DWORD key1,DWORD key2)
{
	DWORD mykey1 = 0;
	DWORD mykey2 = 0;

	if(NULL == g_myinput)
	{
		WLog_ERR(TAG, "Keyboard_init is faild");
		return;
	}
	switch (key1)
	{
		case KEY_WIN:
		{
			mykey1 = RDP_SCANCODE_LWIN;
			if(KEY_R == key2)
				mykey2 = RDP_SCANCODE_KEY_R;
			else if(KEY_D == key2)
				mykey2 = RDP_SCANCODE_KEY_D;
			else
				WLog_ERR(TAG, "undefined KEY_WIN + !!!");
			
			break;
		}
		case KEY_CTRL:
		{
			mykey1 = RDP_SCANCODE_LCONTROL;
			if(KEY_A == key2)
				mykey2 = RDP_SCANCODE_KEY_A;
			else if(KEY_C == key2)
				mykey2 = RDP_SCANCODE_KEY_C;
			else if(KEY_V == key2)
				mykey2 = RDP_SCANCODE_KEY_V;
			else
				WLog_ERR(TAG, "undefined KEY_CTRL + !!!");
			break;
		}
		case KEY_ALT:
		{
			mykey1 = RDP_SCANCODE_LMENU;
			if(KEY_R == key2)
				mykey2 = RDP_SCANCODE_KEY_R;
			else
				WLog_ERR(TAG, "undefined KEY_ALT + !!!");
			break;
		}
		case KEY_SHIFT:
		{
			mykey1 = RDP_SCANCODE_LSHIFT;
			break;
		}
	}
	freerdp_input_send_keyboard_event_ex(g_myinput, TRUE, mykey1);
	freerdp_input_send_keyboard_event_ex(g_myinput, TRUE, mykey2);
	Sleep(100);
	freerdp_input_send_keyboard_event_ex(g_myinput, FALSE, mykey2);
	freerdp_input_send_keyboard_event_ex(g_myinput, FALSE, mykey1);

	return;
}

void SelfSendKeyboard(int type,unsigned int mykey1,unsigned int mykey2)
{
	DWORD key1 = 0;
	DWORD key2 = 0;


	if(ONEKEY == type)
	{
		key1 = mykey1;
		SelfSendOnekey(key1);
	}
	else if(DOUBOLEKEY == type)
	{
		key1 = mykey1;
		key2 = mykey2;

		SelfSendDoublekey(key1,key2);
	}
	else
		WLog_ERR(TAG, "undefined key type!!!");

	return;
}


void Keyboard_init(wfContext* context)
{
	g_myinput = context->context.input;
	if(NULL == g_myinput)
		WLog_ERR(TAG, "Keyboard_init is faild");
	return;
}

DWORD WINAPI excuce(rdpContext* context)
{
	rdpSettings* settings;
	CHAR* startflag = NULL;
	CHAR* endflag = NULL;
	wfContext* wfc = NULL;
	BOOL ret = TRUE;
	BOOL checkstart = FALSE;
	UINT32 cont = 0;
	UINT32 waittime = 0;
	time_t begintime = 0;
	time_t endtime = 0;
	time_t nowtime = 0;
	
	flushlog();
	if (context)
	{
		wfc = (wfContext*) context;
		settings = context->settings;
	} 
	else
	{
		WLog_ERR(TAG,"param context is NULL !!!");
		return FALSE;
	}
	//while(!EXCUCESTARTFLAGE)
	//	Sleep(2000);

	if (!get_startflag(settings, &startflag))
	{
		ret = FALSE;
		goto out;
	}
		


	if (!get_endflag(settings, &endflag))
	{
		ret = FALSE;
		goto out;
	}
	Sleep(1000);
	SelfSendKeyboard(DOUBOLEKEY,KEY_WIN,KEY_D);
	WLog_INFO(TAG, "send Win + D !!!");
	SelfSendKeyboard(ONEKEY,KEY_LEFT,0);
	WLog_INFO(TAG, "send left  !!!");
	Sleep(1000); 
	SelfSendKeyboard(ONEKEY,KEY_ENTER,0);
	WLog_INFO(TAG, "send enter  !!!");
	WLog_INFO(TAG, "wait for windows state up !!!");
	Sleep(2000); 
	;
	//剪切板判断,等待3次失败认为失败
	cont = 0;
	while (!settings->ClipboardDone)
	{
		WLog_ERR(TAG, "剪切板设置失败！");
		if(cont++ <=3)
		{
			Sleep(1000);
			continue;
		}
		else
		{
			ret = FALSE;
			goto out;
		};
			
	}

	//磁盘映射判断,等待3次失败认为失败
	cont = 0;
	while (!settings->RedirectDriveAlready)
	{
		WLog_ERR(TAG, "磁盘映射设置失败！");
		if(cont++ <=3)
		{
			Sleep(2000);
			continue;
		}
		else
		{
			ret = FALSE;
			goto out;
		}

	}
	flushlog();
	if(pre_check_flag(startflag) || pre_check_flag(endflag))
	{
		WLog_ERR(TAG,"[VM_Finished] 标志文件创建时间异常，请清除已有的标志文件，重新运行扫描脚本！ !!!");
		ret = FALSE;
		goto out;
	}


	flushlog();
	SelfSendKeyboard(DOUBOLEKEY,KEY_WIN,KEY_D);
	WLog_INFO(TAG, "send Win + D !!!");
	Sleep(3000);
	SelfSendKeyboard(ONEKEY,KEY_ESC,0);
	WLog_INFO(TAG, "send Esc !!!");
	Sleep(3000); 
	SelfSendKeyboard(ONEKEY,KEY_ESC,0);
	WLog_INFO(TAG, "send Esc !!!");
	flushlog();
	Sleep(3000); 
	SelfSendKeyboard(DOUBOLEKEY,KEY_WIN,KEY_R);
	WLog_INFO(TAG, "send Win + R !!!");
	Sleep(3000);
	SelfSendKeyboard(DOUBOLEKEY,KEY_CTRL,KEY_A);
	WLog_INFO(TAG, "send CTRL + A !!!");
	Sleep(3000);
	SelfSendKeyboard(ONEKEY,KEY_BACK,0);
	WLog_INFO(TAG, "send Backspace !!!");
	flushlog();
	Sleep(3000); 
	SelfSendKeyboard(DOUBOLEKEY,KEY_CTRL,KEY_V);
	WLog_INFO(TAG, "send CTRL + V !!!");
	Sleep(3000);
	SelfSendKeyboard(ONEKEY,KEY_ENTER,0);
	WLog_INFO(TAG, "send ENTER!!!");
	Sleep((settings->BeforeAltR)*100);
	SelfSendKeyboard(DOUBOLEKEY,KEY_ALT,KEY_R);
	WLog_INFO(TAG, "send ALT + R  !!!");
	flushlog();
	settings->WaitingCount;
	time(&begintime);

	cont = 0;
	while(!pre_check_flag(endflag))
	{
		if((!checkstart) && pre_check_flag(startflag))
		{
			checkstart = TRUE;
			time(&begintime);
		}
			
		
		time(&nowtime);
		
		if(nowtime - begintime > settings->MaxTime)
		{
			if (checkstart)
				WLog_ERR(TAG, "[VM_Finished] 远程桌面扫描超时[脚本未执行完毕]，请检查超时时间后重新运行扫描脚本！");
			else
				WLog_ERR(TAG, "[VM_Finished] 远程桌面扫描超时[脚本未执行]，请检查后重新运行扫描脚本：（1）共享磁盘路径错误 （2）远程主机rdpclip.exe进程被杀死（3）“Alt+R”键无效");
			
			ret = FALSE;
			goto out;
		}

		if(!checkstart && (cont++ < 3))
		{
			SelfSendKeyboard(DOUBOLEKEY,KEY_ALT,KEY_R);
			WLog_INFO(TAG, "send ALT + R  !!!");
			Sleep((settings->WaitingCount) * 100);
		}
		else
		{
			Sleep(2000);
		}
	}
	time(&endtime);
	WLog_INFO(TAG, "[VM_Finished] 扫描已完成！脚本运行时间为：%d s.", (int)(endtime - begintime));
	
	
	
out:
	flushlog();
	if (!startflag)
	{
		free(startflag);
	}

	if (!endflag)
	{
		free(endflag);
	}
	

	PostMessage(wfc->hwnd,WM_CLOSE,0,0);

	

	
	return ret;

}

