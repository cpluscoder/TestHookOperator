#include "StdAfx.h"
#include "SetWinEvtHook.h"

#define TEST_CASE_1	0
#define TEST_CASE_2	1

#if	TEST_CASE_1
#include <OleAcc.h>
#pragma comment (lib, "Oleacc.lib")
#endif

CSetWinEvtHook::CSetWinEvtHook(void)
{
	::CoInitialize(NULL);/// 必须先执行

	InitializeMSAA();
}


CSetWinEvtHook::~CSetWinEvtHook(void)
{
	ShutdownMSAA();

	::CoUninitialize();/// 必须最后执行
}

void CSetWinEvtHook::HandleWinEvent(
	HWINEVENTHOOK hook, 
	DWORD dwEvent, 
	HWND hWnd, 
	LONG idObject, 
	LONG idChild, 
	DWORD dwEventThread, 
	DWORD dwmsEventTime)
{
#if TEST_CASE_1
	IAccessible* pAcc = NULL;
	VARIANT varChild;
	HRESULT hr = ::AccessibleObjectFromEvent(hWnd, idObject, idChild, &pAcc, &varChild);
	if((hr == S_OK) && (pAcc != NULL))
	{
		BSTR bstrName;
		pAcc->get_accName(varChild, &bstrName);
		if(dwEvent == EVENT_SYSTEM_MENUSTART)
		{
			printf("Begin: ");
		}
		else if(dwEvent == EVENT_SYSTEM_MENUEND)
		{
			printf("End:   ");
		}
		printf("%S\n", bstrName);
		SysFreeString(bstrName);
		pAcc->Release();
	}
#elif TEST_CASE_2
	switch(dwEvent)
	{
	case EVENT_SYSTEM_SWITCHSTART:
		OutputDebugString(_T("Alt+Tab Start"));
		break;
	case EVENT_SYSTEM_SWITCHEND:
		OutputDebugString(_T("Alt+Tab End"));
		break;
	case EVENT_SYSTEM_MENUPOPUPSTART:
		OutputDebugString(_T("PopMenu Start"));
		break;
	case EVENT_SYSTEM_MENUPOPUPEND:
		OutputDebugString(_T("PopMenu End"));
		break;
	default:
		break;
	}  
#endif
}

void CSetWinEvtHook::InitializeMSAA()
{
	m_hook = ::SetWinEventHook(
		EVENT_SYSTEM_MENUSTART, EVENT_SYSTEM_MENUEND,  // Range of events (4 to 5).
		NULL,                                          // Handle to DLL.
		HandleWinEvent,                                // The callback.
		0, 0,              // Process and thread IDs of interest (0 = all)
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.
}

void CSetWinEvtHook::ShutdownMSAA(void)
{
	if(m_hook != NULL)
	{
		::UnhookWinEvent(m_hook);
		m_hook = NULL;
	}
}
