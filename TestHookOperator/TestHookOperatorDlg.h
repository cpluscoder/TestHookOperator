
// TestHookOperatorDlg.h : 头文件
//

#pragma once

#include "../HookOperator/HookOperator.h"

// CTestHookOperatorDlg 对话框
class CTestHookOperatorDlg : public CDialogEx
{
protected:
	void OnClickMenu(UINT nMenuId, HookType emType);

// 构造
public:
	CTestHookOperatorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTHOOKOPERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
