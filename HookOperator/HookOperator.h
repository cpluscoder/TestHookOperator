// HookOperator.h : HookOperator DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CHookOperatorApp
// 有关此类实现的信息，请参阅 HookOperator.cpp
//

class CHookOperatorApp : public CWinApp
{
public:
	CHookOperatorApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/**
SetWindowsHookEx:截取WM_XXX消息
	全局钩子(global hook):能够截取所有线程的消息, 钩子函数必须在dll中实现, 必须被注入到目标进程
	线程钩子(thread hook):只能截取当前进程的线程消息，钩子函数不需要放在dll中
SetWinEventHook:截取EVENT_XXX消息, 能截取到所有进程的消息, 不能截取键鼠消息, 进程内钩子效率更高
	进程内钩子(WINEVENT_INCONTEXT):必须放在dll中，被映射到所有进程
	进程外钩子(WINEVENT_OUTOFCONTEXT):不会被映射到别的进程，不需要在dll中实现
	可以无视WIN7及以上的UAC机制, 也就是它是给自动测试和残障工具用的，所以它要保证有效
*/

#ifdef HOOK_OPERATOR_EXPORTS
	#define HOOK_OPERATOR_API __declspec(dllexport)
#else
	#define HOOK_OPERATOR_API __declspec(dllimport)
#endif

typedef enum _HookType
{
	emCallWndProc = 0,
	emCBT,
	emDebug,
	emGetMessage,
	emKeyboard,
	emMouse,
	emMsgFilter,

	HookTypeCount
}HookType;


#ifdef __cplusplus
extern "C" 
{
#endif

///	SetWinEventHook
	HOOK_OPERATOR_API bool StartHook(void);
	HOOK_OPERATOR_API void StopHook(void);

///	SetWindowsHookEx
	HOOK_OPERATOR_API void InitializeHookData(HWND hMainWnd);
	HOOK_OPERATOR_API bool EnableHook(HookType emType, bool bEnable = true);

#ifdef __cplusplus
}
#endif

