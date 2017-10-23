#include "StdAfx.h"
#include "SetWinEvtHook.h"
#include "HookOperator.h"

#include <string>

#define WIN32_HANDLE_EVT	1
#define ACC_HANDLE_EVT		0


#if	ACC_HANDLE_EVT
#include <OleAcc.h>
#pragma comment (lib, "Oleacc.lib")
#endif

using namespace std;

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
#if ACC_HANDLE_EVT
	try
	{
		IAccessible* pAcc = NULL;
		VARIANT varChild;
		HRESULT hr = ::AccessibleObjectFromEvent(hWnd, idObject, idChild, &pAcc, &varChild);
		if(hr == S_OK && pAcc != NULL)
		{
			BSTR bstrName;
			pAcc->get_accName(varChild, &bstrName);
			BSTR bstrValue;
			pAcc->get_accValue(varChild, &bstrValue);
			BSTR bstrDesc;
			pAcc->get_accDescription(varChild, &bstrDesc);
			CString strMsg;
			if(dwEvent == EVENT_OBJECT_SHOW/*EVENT_SYSTEM_MENUSTART*/)
			{
				strMsg.Format("Show -- Name:[%S] Value:[%S] Desc:[%S]\n", bstrName, bstrValue, bstrDesc);
				OutputDebugString(strMsg);
			}
			else if(dwEvent == EVENT_OBJECT_HIDE/*EVENT_SYSTEM_MENUEND*/)
			{
				strMsg.Format("HIDE -- Name:[%S] Value:[%S] Desc:[%S]\n", bstrName, bstrValue, bstrDesc);
				OutputDebugString(strMsg);
			}
			
			SysFreeString(bstrName);
			SysFreeString(bstrValue);
			SysFreeString(bstrDesc);
			pAcc->Release();
		}
	}
	catch (CMemoryException* e)
	{
		char szError[256];
		ZeroMemory(szError, sizeof(szError));
		e->GetErrorMessage(szError, sizeof(szError));
		OutputDebugString(szError);
	}
	catch (CFileException* e)
	{
		char szError[256];
		ZeroMemory(szError, sizeof(szError));
		e->GetErrorMessage(szError, sizeof(szError));
		OutputDebugString(szError);
	}
	catch (CException* e)
	{
		char szError[256];
		ZeroMemory(szError, sizeof(szError));
		e->GetErrorMessage(szError, sizeof(szError));
		OutputDebugString(szError);
	}
#elif WIN32_HANDLE_EVT
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

			str.Format(TEXT("Thread:[%d] Caption:[%s] ClassName[%s] Obj:[%x] Child:[%d] SHOW\n"), dwEventThread, szName, className, idObject, idChild);
			OutputDebugString(str);
			//CWindowEventDispatcher::instance()->CheckAndNotify(hWnd);

			string strCaption = "提示";
			string strClassName = "TCustomBaseFormDlg";
			if(strCaption == szName && strClassName == className)
			{
				OutputDebugString("PostMessage close");
				::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			}
		}
		break;
	case EVENT_OBJECT_HIDE:
		{
			CString str;

			TCHAR szName[MAX_PATH] = { 0 };
			::GetWindowText(hWnd, szName, MAX_PATH);
			TCHAR className[MAX_PATH] = { 0 };
			::RealGetWindowClass(hWnd, className, MAX_PATH);

			str.Format(TEXT("Thread:[%d] Caption:[%s] ClassName[%s] Obj:[%x] Child:[%d] HIDE\n"), dwEventThread, szName, className, idObject, idChild);
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
#if ACC_HANDLE_EVT
		EVENT_OBJECT_SHOW, EVENT_OBJECT_HIDE,	//EVENT_SYSTEM_MENUSTART, EVENT_SYSTEM_MENUEND,  // Range of events (4 to 5).
#elif WIN32_HANDLE_EVT
		EVENT_OBJECT_SHOW, EVENT_OBJECT_HIDE,	//EVENT_MIN, EVENT_MAX,
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


CSetWinEvtHook *g_pSetWinEvtHook;

void StopHook(void)
{
	if(g_pSetWinEvtHook != NULL)
	{
		delete g_pSetWinEvtHook;
		g_pSetWinEvtHook = NULL;
	}
}

bool StartHook(void)
{
	StopHook();

	g_pSetWinEvtHook = new CSetWinEvtHook;

	return true;
}

