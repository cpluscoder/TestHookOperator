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
