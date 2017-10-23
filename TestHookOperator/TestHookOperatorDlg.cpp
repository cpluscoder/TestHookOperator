
// TestHookOperatorDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTestHookOperatorDlg �Ի���




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


// CTestHookOperatorDlg ��Ϣ�������

BOOL CTestHookOperatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitializeHookData(GetSafeHwnd());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestHookOperatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestHookOperatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestHookOperatorDlg::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_START_HOOK)->EnableWindow(FALSE);
	StartHook();
	GetDlgItem(IDC_BTN_STOP_HOOK)->EnableWindow(TRUE);
}


void CTestHookOperatorDlg::OnBnClickedBtnStopHook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_START_HOOK)->EnableWindow(TRUE);
	StopHook();
	GetDlgItem(IDC_BTN_STOP_HOOK)->EnableWindow(FALSE);
}


void CTestHookOperatorDlg::OnBnClickedBtnMsgbox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM1, emCallWndProc);
}


void CTestHookOperatorDlg::OnMenuItem2()
{
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM2, emCBT);
}


void CTestHookOperatorDlg::OnMenuItem3()
{
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM3, emDebug);
}


void CTestHookOperatorDlg::OnMenuItem4()
{
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM4, emGetMessage);
}


void CTestHookOperatorDlg::OnMenuItem5()
{
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM5, emKeyboard);
}


void CTestHookOperatorDlg::OnMenuItem6()
{
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM6, emMouse);
}


void CTestHookOperatorDlg::OnMenuItem7()
{
	// TODO: �ڴ���������������
	OnClickMenu(ID_MENU_ITEM7, emMsgFilter);
}
