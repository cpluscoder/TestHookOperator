#pragma once

class CSetWinEvtHook
{
public:
	CSetWinEvtHook(void);

	// Unhooks the event and shuts down COM.
	//
	virtual ~CSetWinEvtHook(void);

	// Callback function that handles events.
	//
	static void CALLBACK HandleWinEvent(
		HWINEVENTHOOK hook, 
		DWORD dwEvent, 
		HWND hWnd, 
		LONG idObject, 
		LONG idChild, 
		DWORD dwEventThread, 
		DWORD dwmsEventTime);

protected:
	void Initialize(void);

	void Shutdown(void);


protected:
	HWINEVENTHOOK m_hook;
};

