
// TestHookOperatorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestHookOperator.h"
#include "TestHookOperatorDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment(lib, "HookOperator100d.lib")
#else
#pragma comment(lib, "HookOperator100.lib")
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestHookOperatorDlg 对话框




CTestHookOperatorDlg::CTestHookOperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestHookOperatorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestHookOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestHookOperatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_HOOK, &CTestHookOperatorDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_STOP_HOOK, &CTestHookOperatorDlg::OnBnClickedBtnStopHook)
	ON_BN_CLICKED(IDC_BTN_MSGBOX, &CTestHookOperatorDlg::OnBnClickedBtnMsgbox)
	ON_COMMAND(ID_MENU_ITEM1, &CTestHookOperatorDlg::OnMenuItem1)
	ON_COMMAND(ID_MENU_ITEM2, &CTestHookOperatorDlg::OnMenuItem2)
	ON_COMMAND(ID_MENU_ITEM3, &CTestHookOperatorDlg::OnMenuItem3)
	ON_COMMAND(ID_MENU_ITEM4, &CTestHookOperatorDlg::OnMenuItem4)
	ON_COMMAND(ID_MENU_ITEM5, &CTestHookOperatorDlg::OnMenuItem5)
	ON_COMMAND(ID_MENU_ITEM6, &CTestHookOperatorDlg::OnMenuItem6)
	ON_COMMAND(ID_MENU_ITEM7, &CTestHookOperatorDlg::OnMenuItem7)
END_MESSAGE_MAP()


// CTestHookOperatorDlg 消息处理程序

BOOL CTestHookOperatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitializeHookData(GetSafeHwnd());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestHookOperatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestHookOperatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestHookOperatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestHookOperatorDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_START_HOOK)->EnableWindow(FALSE);
	StartHook();
	GetDlgItem(IDC_BTN_STOP_HOOK)->EnableWindow(TRUE);
}


void CTestHookOperatorDlg::OnBnClickedBtnStopHook()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_START_HOOK)->EnableWindow(TRUE);
	StopHook();
	GetDlgItem(IDC_BTN_STOP_HOOK)->EnableWindow(FALSE);
}


void CTestHookOperatorDlg::OnBnClickedBtnMsgbox()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox("Test");
}

void CTestHookOperatorDlg::OnClickMenu(UINT nMenuId, HookType emType)
{
	CMenu *pMenu = GetMenu()->GetSubMenu(0);
	UINT nState = pMenu->GetMenuState(nMenuId, MF_BYCOMMAND);
	bool bEnable = (nState & MF_CHECKED) != 0;
	if(EnableHook(emType, !bEnable))
	{
		UINT nCheck = bEnable ? MF_UNCHECKED | MF_BYCOMMAND : MF_CHECKED | MF_BYCOMMAND;
		pMenu->CheckMenuItem(nMenuId, nCheck);
	}
}

void CTestHookOperatorDlg::OnMenuItem1()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM1, emCallWndProc);
}


void CTestHookOperatorDlg::OnMenuItem2()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM2, emCBT);
}


void CTestHookOperatorDlg::OnMenuItem3()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM3, emDebug);
}


void CTestHookOperatorDlg::OnMenuItem4()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM4, emGetMessage);
}


void CTestHookOperatorDlg::OnMenuItem5()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM5, emKeyboard);
}


void CTestHookOperatorDlg::OnMenuItem6()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM6, emMouse);
}


void CTestHookOperatorDlg::OnMenuItem7()
{
	// TODO: 在此添加命令处理程序代码
	OnClickMenu(ID_MENU_ITEM7, emMsgFilter);
}
