// HookOperator.h : HookOperator DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHookOperatorApp
// �йش���ʵ�ֵ���Ϣ������� HookOperator.cpp
//

class CHookOperatorApp : public CWinApp
{
public:
	CHookOperatorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/**
SetWindowsHookEx:��ȡWM_XXX��Ϣ
	ȫ�ֹ���(global hook):�ܹ���ȡ�����̵߳���Ϣ, ���Ӻ���������dll��ʵ��, ���뱻ע�뵽Ŀ�����
	�̹߳���(thread hook):ֻ�ܽ�ȡ��ǰ���̵��߳���Ϣ�����Ӻ�������Ҫ����dll��
SetWinEventHook:��ȡEVENT_XXX��Ϣ, �ܽ�ȡ�����н��̵���Ϣ, ���ܽ�ȡ������Ϣ, �����ڹ���Ч�ʸ���
	�����ڹ���(WINEVENT_INCONTEXT):�������dll�У���ӳ�䵽���н���
	�����⹳��(WINEVENT_OUTOFCONTEXT):���ᱻӳ�䵽��Ľ��̣�����Ҫ��dll��ʵ��
	��������WIN7�����ϵ�UAC����, Ҳ�������Ǹ��Զ����ԺͲ��Ϲ����õģ�������Ҫ��֤��Ч
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

