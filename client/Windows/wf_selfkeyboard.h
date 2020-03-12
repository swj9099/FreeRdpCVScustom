/**

 **/

#ifndef WF_SELFKEYBOARD_H
#define WF_SELFKEYBOARD_H

#include <winpr/windows.h>

#include <winpr/collections.h>
#include "freerdp/freerdp.h"
#include "wf_client.h"

#ifdef __cplusplus
extern "C" {
#endif



#define ONEKEY 0
#define DOUBOLEKEY 1

enum KEY_SELFTYPE
{
	KEY_ENTER=0,
	KEY_ESC,
	KEY_WIN,
	KEY_BACK,
	KEY_CTRL,
	KEY_ALT,
	KEY_SHIFT,
	KEY_A,
	KEY_C,
	KEY_D,
	KEY_V,
	KEY_R
};


BOOL pre_check_flag(char* filename);

void SelfSendOnekey(DWORD key1);

void SelfSendDoublekey(DWORD key1,DWORD key2);

void SelfSendKeyboard(int type,unsigned int mykey1,unsigned int mykey2);

void Keyboard_init(wfContext* context);

DWORD WINAPI excuce(rdpContext* context);

#ifdef __cplusplus
}
#endif

#endif /* WF_SELFKEYBOARD_H */
