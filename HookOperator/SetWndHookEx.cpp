#include "StdAfx.h"
//#include "SetWndHookEx.h"

#include "HookOperator.h"


#include <windows.h>
#include <strsafe.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")



BOOL afHooks[HookTypeCount];

// Global variables

typedef struct _MyHookData
{
	int nType;
	HOOKPROC hkprc;
	HHOOK hHook;
}MyHookData;
MyHookData g_arrHookData[HookTypeCount];

HWND g_hMainWnd;

// Hook procedures
LRESULT WINAPI CallWndProc(int, WPARAM, LPARAM);
LRESULT WINAPI CBTProc(int, WPARAM, LPARAM);
LRESULT WINAPI DebugProc(int, WPARAM, LPARAM);
LRESULT WINAPI GetMsgProc(int, WPARAM, LPARAM);
LRESULT WINAPI KeyboardProc(int, WPARAM, LPARAM);
LRESULT WINAPI MouseProc(int, WPARAM, LPARAM);
LRESULT WINAPI MessageProc(int, WPARAM, LPARAM);

#define MSG_LENGTH_LEN	128

void LookUpTheMessage(PMSG pMsg, LPTSTR strMsg)
{
	sprintf_s(strMsg, MSG_LENGTH_LEN, "hWnd[0x%08x] Msg[0x%08x]", pMsg->hwnd, pMsg->message);
}

LRESULT WINAPI MainWndProc(HWND hMainWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nIndex = 0;

	static BOOL afHooks[HookTypeCount];
	//static HMENU hMenu = nullptr;

	g_hMainWnd = hMainWnd;

	switch (uMsg)
	{
	case WM_CREATE:
		{
			// Save the menu handle
			//hMenu = GetMenu(hMainWnd);

			// Initialize structures with hook data. The menu-item identifiers are defined as 0 through 6 in the header file app.h.
			// They can be used to identify array elements both here and during the WM_COMMAND message.
			g_arrHookData[emCallWndProc].nType = WH_CALLWNDPROC;
			g_arrHookData[emCallWndProc].hkprc = CallWndProc;
			g_arrHookData[emCBT].nType = WH_CBT;
			g_arrHookData[emCBT].hkprc = CBTProc;
			g_arrHookData[emDebug].nType = WH_DEBUG;
			g_arrHookData[emDebug].hkprc = DebugProc;
			g_arrHookData[emGetMessage].nType = WH_GETMESSAGE;
			g_arrHookData[emGetMessage].hkprc = GetMsgProc;
			g_arrHookData[emKeyboard].nType = WH_KEYBOARD;
			g_arrHookData[emKeyboard].hkprc = KeyboardProc;
			g_arrHookData[emMouse].nType = WH_MOUSE;
			g_arrHookData[emMouse].hkprc = MouseProc;
			g_arrHookData[emMsgFilter].nType = WH_MSGFILTER;
			g_arrHookData[emMsgFilter].hkprc = MessageProc;

			// Initialize all flags in the array to FALSE.
			memset(afHooks, FALSE, sizeof(afHooks));

			return 0;
		}
		break;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			// The user selected a hook command from the menu.
			case emCallWndProc:
			case emCBT:
			case emDebug:
			case emGetMessage:
			case emKeyboard:
			case emMouse:
			case emMsgFilter:
				{
					// Use the menu-item identifier as an Index into the array of structures with hook data.
					nIndex = LOWORD(wParam);

					// If the selected type of hook procedure isn't installed yet, install it and check the associated menu item.
					if (!afHooks[nIndex])
					{
						g_arrHookData[nIndex].hHook = SetWindowsHookEx(g_arrHookData[nIndex].nType, g_arrHookData[nIndex].hkprc, (HINSTANCE) NULL, GetCurrentThreadId());

						//CheckMenuItem(hMenu, nIndex, MF_BYCOMMAND | MF_CHECKED);

						afHooks[nIndex] = TRUE;
					}
					// If the selected type of hook procedure is already installed, remove it and remove the check mark from the associated menu item.
					else
					{
						UnhookWindowsHookEx(g_arrHookData[nIndex].hHook);

						//CheckMenuItem(hMenu, nIndex, MF_BYCOMMAND | MF_UNCHECKED);

						afHooks[nIndex] = FALSE;
					}
				}
				break;
			default:
				return (DefWindowProc(hMainWnd, uMsg, wParam, lParam));
			}
		}
		break;

		//
		// Process other messages.
		//

	default:
		return DefWindowProc(hMainWnd, uMsg, wParam, lParam);
	}

	return NULL;
}

/****************************************************************
WH_CALLWNDPROC hook procedure
****************************************************************/
LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// do not process message
	if (nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emCallWndProc].hHook, nCode, wParam, lParam);
	}

	CHAR szCWPBuf[256];
	CHAR szMsg[MSG_LENGTH_LEN];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// Call an application-defined function that converts a message constant to a string and copies it to a buffer.
	LookUpTheMessage((PMSG) lParam, szMsg);

	switch (nCode)
	{
	case HC_ACTION:
		{
			hResult = StringCchPrintf(szCWPBuf, 256/sizeof(TCHAR), "CALLWNDPROC - tsk: %ld, msg: %s, %d times   ", wParam, szMsg, c++);
			if (FAILED(hResult))
			{
				// TODO: writer error handler
			}
			hResult = StringCchLength(szCWPBuf, 256/sizeof(TCHAR), &cch);
			if (FAILED(hResult))
			{
				// TODO: write error handler
			}
			hdc = GetDC(g_hMainWnd);
			RECT rc;
			GetClientRect(g_hMainWnd, &rc);
			rc.bottom -= 200;
			InvalidateRect(g_hMainWnd, &rc, TRUE);
			TextOut(hdc, 2, 15, szCWPBuf, cch);
		}
		break;
	default:
		break;
	}

	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emCallWndProc].hHook, nCode, wParam, lParam);
}


/****************************************************************
WH_CBT hook procedure
****************************************************************/
LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CHAR szBuf[128];
	CHAR szCode[128];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// do not process message
	if(nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emCBT].hHook, nCode, wParam, lParam);
	}

	hdc = GetDC(g_hMainWnd);

	switch (nCode)
	{
	case HCBT_ACTIVATE:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_ACTIVATE");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_CLICKSKIPPED:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_CLICKSKIPPED");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_CREATEWND:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_CREATEWND");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_DESTROYWND:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_DESTROYWND");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_KEYSKIPPED:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_KEYSKIPPED");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_MINMAX:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_MINMAX");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_MOVESIZE:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_MOVESIZE");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_QS:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_QS");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_SETFOCUS:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_SETFOCUS");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case HCBT_SYSCOMMAND:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "HCBT_SYSCOMMAND");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	default:
		hResult = StringCchCopy(szCode, 128/sizeof(TCHAR), "Unknown");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;
	}
	hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), "CBT -  nCode: %s, tsk: %ld, %d times   ", szCode, wParam, c++);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	TextOut(hdc, 2, 75, szBuf, cch);
	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emCBT].hHook, nCode, wParam, lParam);
}


/****************************************************************
WH_DEBUG hook procedure
****************************************************************/
LRESULT CALLBACK DebugProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CHAR szBuf[128];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// do not process message
	if (nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emDebug].hHook, nCode, wParam, lParam);
	}

	hdc = GetDC(g_hMainWnd);

	switch (nCode)
	{
	case HC_ACTION:
		hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), "DEBUG - nCode: %d, tsk: %ld, %d times   ", nCode,wParam, c++);
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		TextOut(hdc, 2, 55, szBuf, cch);
		break;

	default:
		break;
	}

	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emDebug].hHook, nCode, wParam, lParam);
}

/****************************************************************
WH_GETMESSAGE hook procedure
****************************************************************/
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CHAR szMSGBuf[256];
	CHAR szRem[16];
	CHAR szMsg[MSG_LENGTH_LEN];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// do not process message
	if (nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emGetMessage].hHook, nCode, wParam, lParam);
	}

	switch (nCode)
	{
	case HC_ACTION:
		switch (wParam)
		{
		case PM_REMOVE:
			hResult = StringCchCopy(szRem, 16/sizeof(TCHAR), "PM_REMOVE");
			if (FAILED(hResult))
			{
				// TODO: write error handler
			}
			break;

		case PM_NOREMOVE:
			hResult = StringCchCopy(szRem, 16/sizeof(TCHAR), "PM_NOREMOVE");
			if (FAILED(hResult))
			{
				// TODO: write error handler
			}
			break;

		default:
			hResult = StringCchCopy(szRem, 16/sizeof(TCHAR), "Unknown");
			if (FAILED(hResult))
			{
				// TODO: write error handler
			}
			break;
		}

		// Call an application-defined function that converts a message constant to a string and copies it to a buffer.
		LookUpTheMessage((PMSG) lParam, szMsg);

		hdc = GetDC(g_hMainWnd);
		hResult = StringCchPrintf(szMSGBuf, 256/sizeof(TCHAR), "GETMESSAGE - wParam: %s, msg: %s, %d times   ", szRem, szMsg, c++);
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		hResult = StringCchLength(szMSGBuf, 256/sizeof(TCHAR), &cch);
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		TextOut(hdc, 2, 35, szMSGBuf, cch);
		break;

	default:
		break;
	}

	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emGetMessage].hHook, nCode, wParam, lParam);
}


/****************************************************************
WH_KEYBOARD hook procedure
****************************************************************/
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CHAR szBuf[128];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// do not process message
	if (nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emKeyboard].hHook, nCode, wParam, lParam);
	}

	hdc = GetDC(g_hMainWnd);
	hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), "KEYBOARD - nCode: %d, vk: %d, %d times ", nCode, wParam, c++);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	TextOut(hdc, 2, 115, szBuf, cch);
	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emKeyboard].hHook, nCode, wParam, lParam);
}


/****************************************************************
WH_MOUSE hook procedure
****************************************************************/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CHAR szBuf[128];
	CHAR szMsg[MSG_LENGTH_LEN];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// do not process the message
	if (nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emMouse].hHook, nCode, wParam, lParam);
	}

	// Call an application-defined function that converts a message constant to a string and copies it to a buffer.
	LookUpTheMessage((PMSG) lParam, szMsg);

	hdc = GetDC(g_hMainWnd);
	hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), "MOUSE - nCode: %d, msg: %s, x: %d, y: %d, %d times   ", nCode, szMsg, LOWORD(lParam), HIWORD(lParam), c++);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	TextOut(hdc, 2, 95, szBuf, cch);
	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emMouse].hHook, nCode, wParam, lParam);
}

/****************************************************************
WH_MSGFILTER hook procedure
****************************************************************/
LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CHAR szBuf[128];
	CHAR szMsg[MSG_LENGTH_LEN];
	CHAR szCode[32];
	HDC hdc;
	static int c = 0;
	size_t cch;
	HRESULT hResult;

	// do not process message
	if (nCode < 0)
	{
		return CallNextHookEx(g_arrHookData[emMsgFilter].hHook, nCode, wParam, lParam);
	}

	switch (nCode)
	{
	case MSGF_DIALOGBOX:
		hResult = StringCchCopy(szCode, 32/sizeof(TCHAR), "MSGF_DIALOGBOX");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case MSGF_MENU:
		hResult = StringCchCopy(szCode, 32/sizeof(TCHAR), "MSGF_MENU");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	case MSGF_SCROLLBAR:
		hResult = StringCchCopy(szCode, 32/sizeof(TCHAR), "MSGF_SCROLLBAR");
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;

	default:
		hResult = StringCchPrintf(szCode, 128/sizeof(TCHAR), "Unknown: %d", nCode);
		if (FAILED(hResult))
		{
			// TODO: write error handler
		}
		break;
	}

	// Call an application-defined function that converts a message constant to a string and copies it to a buffer.
	LookUpTheMessage((PMSG) lParam, szMsg);

	hdc = GetDC(g_hMainWnd);
	hResult = StringCchPrintf(szBuf, 128/sizeof(TCHAR), "MSGFILTER  nCode: %s, msg: %s, %d times    ", szCode, szMsg, c++);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	hResult = StringCchLength(szBuf, 128/sizeof(TCHAR), &cch);
	if (FAILED(hResult))
	{
		// TODO: write error handler
	}
	TextOut(hdc, 2, 135, szBuf, cch);
	ReleaseDC(g_hMainWnd, hdc);

	return CallNextHookEx(g_arrHookData[emMsgFilter].hHook, nCode, wParam, lParam);
}

void InitializeHookData(HWND hMainWnd)
{
	g_hMainWnd = hMainWnd;

	// Initialize all flags in the array to FALSE.
	memset(afHooks, FALSE, sizeof(afHooks));

	g_arrHookData[emCallWndProc].nType = WH_CALLWNDPROC;
	g_arrHookData[emCallWndProc].hkprc = CallWndProc;

	g_arrHookData[emCBT].nType = WH_CBT;
	g_arrHookData[emCBT].hkprc = CBTProc;

	g_arrHookData[emDebug].nType = WH_DEBUG;
	g_arrHookData[emDebug].hkprc = DebugProc;

	g_arrHookData[emGetMessage].nType = WH_GETMESSAGE;
	g_arrHookData[emGetMessage].hkprc = GetMsgProc;

	g_arrHookData[emKeyboard].nType = WH_KEYBOARD;
	g_arrHookData[emKeyboard].hkprc = KeyboardProc;

	g_arrHookData[emMouse].nType = WH_MOUSE;
	g_arrHookData[emMouse].hkprc = MouseProc;

	g_arrHookData[emMsgFilter].nType = WH_MSGFILTER;
	g_arrHookData[emMsgFilter].hkprc = MessageProc;
}

bool EnableHook(HookType emType, bool bEnable)
{
	CString strMsg;
	switch(emType)
	{
	case emCallWndProc:
		{
			strMsg = "Hook:CallWndProc";
		}
		break;
	case emCBT:
		{
			strMsg = "Hook:CBT";
		}
		break;
	case emDebug:
		{
			strMsg = "Hook:Debug";
		}
		break;
	case emGetMessage:
		{
			strMsg = "Hook:GetMessage";
		}
		break;
	case emKeyboard:
		{
			strMsg = "Hook:Keyboard";
		}
		break;
	case emMouse:
		{
			strMsg = "Hook:Mouse";
		}
		break;
	case emMsgFilter:
		{
			strMsg = "Hook:MsgFilter";
		}
		break;
	default:
		{
			OutputDebugString("Î´ÖªHOOKÀàÐÍ");
			return false;
		}
	}
	

	// Use the menu-item identifier as an Index into the array of structures with hook data.
	int nIndex = emType;

	// If the selected type of hook procedure isn't installed yet, install it and check the associated menu item.
	if (!afHooks[nIndex])
	{
		if(bEnable)
		{
			g_arrHookData[nIndex].hHook = ::SetWindowsHookEx(g_arrHookData[nIndex].nType, g_arrHookData[nIndex].hkprc, (HINSTANCE) NULL, GetCurrentThreadId());
			afHooks[nIndex] = TRUE;
			strMsg.Append(" Enable");
			OutputDebugString(strMsg);
			return true;
		}
	}
	// If the selected type of hook procedure is already installed, remove it and remove the check mark from the associated menu item.
	else
	{
		if(!bEnable)
		{
			::UnhookWindowsHookEx(g_arrHookData[nIndex].hHook);
			afHooks[nIndex] = FALSE;
			strMsg.Append(" Disable");
			OutputDebugString(strMsg);
			return true;
		}
	}

	return false;
}
