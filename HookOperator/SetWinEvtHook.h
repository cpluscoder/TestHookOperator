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
	// Initializes COM and sets up the event hook.
	//
	void InitializeMSAA(void);

	// Unhooks the event and shuts down COM.
	//
	void ShutdownMSAA(void);


protected:
	HWINEVENTHOOK m_hook;
};

