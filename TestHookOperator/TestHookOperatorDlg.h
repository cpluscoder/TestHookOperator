
// TestHookOperatorDlg.h : ͷ�ļ�
//

#pragma once

#include "../HookOperator/HookOperator.h"

// CTestHookOperatorDlg �Ի���
class CTestHookOperatorDlg : public CDialogEx
{
protected:
	void OnClickMenu(UINT nMenuId, HookType emType);

// ����
public:
	CTestHookOperatorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTHOOKOPERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnStopHook();
	afx_msg void OnBnClickedBtnMsgbox();
	afx_msg void OnMenuItem1();
	afx_msg void OnMenuItem2();
	afx_msg void OnMenuItem3();
	afx_msg void OnMenuItem4();
	afx_msg void OnMenuItem5();
	afx_msg void OnMenuItem6();
	afx_msg void OnMenuItem7();
};
