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
#include "wf_event.h"
#define TAG CLIENT_TAG("windows")

rdpInput* g_myinput = NULL;

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

void excuce()
{
	Sleep(10000);
	SelfSendKeyboard(ONEKEY,KEY_WIN,0);
	Sleep(5000);
	SelfSendKeyboard(ONEKEY,KEY_ESC,0);

}

