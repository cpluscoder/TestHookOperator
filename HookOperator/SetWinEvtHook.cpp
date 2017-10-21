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

	Initialize();
}


CSetWinEvtHook::~CSetWinEvtHook(void)
{
	Shutdown();

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
			TRACE("Begin: ");
		}
		else if(dwEvent == EVENT_SYSTEM_MENUEND)
		{
			TRACE("End:   ");
		}
		TRACE("%S\n", bstrName);
		SysFreeString(bstrName);
		pAcc->Release();
	}
#elif TEST_CASE_2
	switch(dwEvent)
	{
	case EVENT_SYSTEM_SWITCHSTART:
		OutputDebugString(_T("Alt+Tab Start\n"));
		break;
	case EVENT_SYSTEM_SWITCHEND:
		OutputDebugString(_T("Alt+Tab End\n"));
		break;
	case EVENT_SYSTEM_MENUPOPUPSTART:
		OutputDebugString(_T("PopMenu Start\n"));
		break;
	case EVENT_SYSTEM_MENUPOPUPEND:
		OutputDebugString(_T("PopMenu End\n"));
		break;
	case EVENT_OBJECT_SHOW:
		{
			CString str;
			TCHAR szName[MAX_PATH] = { 0 };
			::GetWindowText(hWnd, szName, MAX_PATH);

			TCHAR className[MAX_PATH] = { 0 };
			::RealGetWindowClass(hWnd, className, MAX_PATH);
			//HWND hOwner = ::GetWindow(hWnd, GW_OWNER);
			//HWND hParent = ::GetParent(hWnd);

			str.Format(TEXT("[%d] [%s] [%s] obj:[%x] id:[%d] SHOW\n"), dwEventThread, szName, className, idObject, idChild);
			OutputDebugString(str);
			//CWindowEventDispatcher::instance()->CheckAndNotify(hWnd);
		}
		break;
	case EVENT_OBJECT_HIDE:
		{
			CString str;

			TCHAR szName[MAX_PATH] = { 0 };
			::GetWindowText(hWnd, szName, MAX_PATH);
			TCHAR className[MAX_PATH] = { 0 };
			::RealGetWindowClass(hWnd, className, MAX_PATH);

			str.Format(TEXT("[%d] [%s] [%s] obj:[%x] id:[%d] HIDE"), dwEventThread, szName, className, idObject, idChild);
			OutputDebugString(str);
		}
		break;
	default:
		{
			//TRACE("Unhandle Event:0x%04x\n", dwEvent);
		}
		break;
	}  
#endif
}

void CSetWinEvtHook::Initialize()
{
	m_hook = ::SetWinEventHook(
#if TEST_CASE_1
		EVENT_SYSTEM_MENUSTART, EVENT_SYSTEM_MENUEND,  // Range of events (4 to 5).
#elif TEST_CASE_2
		EVENT_OBJECT_SHOW, EVENT_OBJECT_HIDE,  //EVENT_MIN, EVENT_MAX,
#endif
		NULL,                                          // Handle to DLL.
		HandleWinEvent,                                // The callback.
		0, 0,              // Process and thread IDs of interest (0 = all)
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS); // Flags.
}

void CSetWinEvtHook::Shutdown(void)
{
	if(m_hook != NULL)
	{
		::UnhookWinEvent(m_hook);
		m_hook = NULL;
	}
}
