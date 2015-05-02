
// GetImageDlg.cpp : implement文件
//

#include "stdafx.h"
#include "GetImage.h"
#include "GetImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGetImageDlg dialog




CGetImageDlg::CGetImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetImageDlg::IDD, pParent)
	, m_strEvents(_T(""))
	, m_session(_T("GetImage"))
	//, m_pConnect(NULL)
	//, m_pFile(NULL)
	, m_nThrdNum(0)
	, m_bEcho(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
}

BOOL CGetImageDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccelTable)   
	{   
		if (::TranslateAccelerator(m_hWnd,m_hAccelTable,pMsg))   
		{   
			return TRUE;   
		}   
	}   
	return CDialog::PreTranslateMessage(pMsg);   
}

void CGetImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EVENTS, m_strEvents);
	DDX_Control(pDX, IDC_THREADNUM, m_pcThrdNum);
}

BEGIN_MESSAGE_MAP(CGetImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CGetImageDlg::OnBnClickedOk)
	ON_CBN_SELENDOK(IDC_ADDRESS, &CGetImageDlg::OnCbnSelendokAddress)
	ON_BN_CLICKED(IDC_OPTION, &CGetImageDlg::OnBnClickedOption)
	ON_COMMAND(ID_EDIT_SELECT_ALL,&CGetImageDlg::SelectAll)
	ON_COMMAND(IDC_CLEAR,OnBnClickedClear)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_ABOUT, OnStnClickedAbout)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, SetTipText)
END_MESSAGE_MAP()


// CGetImageDlg 消息处理程序
CGetImageDlg *pDlg;

BOOL CGetImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此dialog的图标。当应用程序主窗口不是dialog时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString tmp;
	tmp.LoadString(IDS_START1); Echo(tmp);
	tmp.LoadString(IDS_START2); Echo(tmp);
	tmp.LoadString(IDS_START3); Echo(tmp);

	m_bStarted = false;
	tmp.LoadString(IDS_STATUS_READY); SetStatus(tmp);

	pDlg = this;
	EnableToolTips();

	CComboBoxEx *pCB = (CComboBoxEx *)GetDlgItem(IDC_ADDRESS);
	tmp.LoadString(IDS_SAMPLE_PAGE1); InsertAddress(pCB, tmp);
	tmp.LoadString(IDS_SAMPLE_PAGE2); InsertAddress(pCB, tmp);
	tmp.LoadString(IDS_SAMPLE_PAGE3); InsertAddress(pCB, tmp);


	//((CComboBox *)pCB)->ModifyStyle(0,CBS_AUTOHSCROLL);
	m_pcThrdNum.SetRange(0,10);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向dialog添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGetImageDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGetImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGetImageDlg::OnSize(UINT nType, int cx, int cy) // 改变窗口大小时，对控件进行移动和缩放
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd *pWnd = GetDlgItem(IDC_EVENTS);
	pWnd->SetWindowPos(NULL, 0,0, cx-21,cy-75, SWP_NOZORDER | SWP_NOMOVE);  

	pWnd = GetDlgItem(IDC_ADDRESS);
	pWnd->SetWindowPos(NULL, 0,0, cx-230,50, SWP_NOZORDER | SWP_NOMOVE);

	pWnd = GetDlgItem(IDOK);
	pWnd->SetWindowPos(NULL, cx-150,13, 0,0, SWP_NOZORDER | SWP_NOSIZE);

	pWnd = GetDlgItem(IDC_OPTION);
	pWnd->SetWindowPos(NULL, cx-80,13, 0,0, SWP_NOZORDER | SWP_NOSIZE);

	pWnd = GetDlgItem(IDC_STATUS);
	pWnd->SetWindowPos(NULL, 6,cy-19,cx-150,18, SWP_NOZORDER);

	pWnd = GetDlgItem(IDC_CLEAR);
	pWnd->SetWindowPos(NULL, cx-80,cy-23, 0,0, SWP_NOZORDER | SWP_NOSIZE);

	pWnd = GetDlgItem(IDC_ABOUT);
	pWnd->SetWindowPos(NULL, cx-120,cy-23, 0,0, SWP_NOZORDER | SWP_NOSIZE);

	pWnd = GetDlgItem(IDC_BMP_THRD);
	pWnd->SetWindowPos(NULL, cx-18,cy-130, 0,0, SWP_NOZORDER | SWP_NOSIZE);

	m_pcThrdNum.SetWindowPos(NULL, cx-20,cy-110, 0,0, SWP_NOZORDER | SWP_NOSIZE);
}

void CGetImageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bStarted)
	{
		m_bStarted = false; // 结束
	}
	else 
	{
		CComboBoxEx *pCB = (CComboBoxEx *)GetDlgItem(IDC_ADDRESS); // 开始
		pCB->GetEditCtrl()->GetWindowText(m_strAddress);
		if (m_strAddress.Left(4) != L"http") m_strAddress = L"http://"+m_strAddress;
		SlashUrl(m_strAddress);
		InsertAddress(pCB,m_strAddress);

		m_bStarted = true;
		CButton *pBtn = (CButton *)GetDlgItem(IDOK);
		pBtn->SetWindowText(L"Stop");

		StartTask();
	}
}

void CGetImageDlg::OnCbnSelendokAddress() // 选中下拉列表中的一项时
{
	/*CComboBoxEx *pCB = (CComboBoxEx *)GetDlgItem(IDC_ADDRESS);
	pCB->GetLBText(pCB->GetCurSel(), m_strAddress);*/
}

void CGetImageDlg::OnBnClickedOption()
{
	// TODO: 在此添加控件通知处理程序代码
	m_optDlg.DoModal();
	m_strImgInc = m_optDlg.m_strImgInc;
	m_strImgInc.MakeLower();
	m_strPageInc = m_optDlg.m_strPageInc;
	m_strPageInc.MakeLower();
	//m_pcThrdNum.SetRange(0,m_optDlg.m_nMaxThread);
}

void CGetImageDlg::OnCancel()
{
	PostQuitMessage(0);
}

void CGetImageDlg::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	/*m_strEvents = L"";
	UpdateData(FALSE);*/
	m_bEcho = !m_bEcho;
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_CLEAR);
	pEdit->SetWindowText(m_bEcho? L"No echo" : L"Echo");
}

HBRUSH CGetImageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID() == IDC_CLEAR || pWnd->GetDlgCtrlID() == IDC_ABOUT)
	{
		pDC->SetTextColor(RGB(0,0,255)); // 蓝色文字
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CGetImageDlg::OnStnClickedAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DlgAbout.DoModal();
}

BOOL CGetImageDlg::SetTipText( UINT id, NMHDR *pTTTStruct, LRESULT *pResult )
{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
		UINT nID = pTTTStruct->idFrom;   //得到相应窗口ID，有可能是HWND   

		//表明nID是否为HWND
		if (pTTT->uFlags & TTF_IDISHWND) // ID IS HWND
		{
			//从HWND得到ID值，当然你也可以通过HWND值来判断
			nID = ::GetDlgCtrlID((HWND)nID);
			if (NULL == nID) return FALSE;

			switch(nID)
			{
			case(IDC_BMP_THRD):
				wcscpy(pTTT->lpszText, L"Number of threads");
				break;
			default:
				break;
			}

			return TRUE;
		}
		return FALSE;   
}