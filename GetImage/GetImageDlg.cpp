
// GetImageDlg.cpp : implement�ļ�
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


// CGetImageDlg ��Ϣ�������
CGetImageDlg *pDlg;

BOOL CGetImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô�dialog��ͼ�ꡣ��Ӧ�ó��������ڲ���dialogʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����dialog�����С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGetImageDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGetImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGetImageDlg::OnSize(UINT nType, int cx, int cy) // �ı䴰�ڴ�Сʱ���Կؼ������ƶ�������
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bStarted)
	{
		m_bStarted = false; // ����
	}
	else 
	{
		CComboBoxEx *pCB = (CComboBoxEx *)GetDlgItem(IDC_ADDRESS); // ��ʼ
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

void CGetImageDlg::OnCbnSelendokAddress() // ѡ�������б��е�һ��ʱ
{
	/*CComboBoxEx *pCB = (CComboBoxEx *)GetDlgItem(IDC_ADDRESS);
	pCB->GetLBText(pCB->GetCurSel(), m_strAddress);*/
}

void CGetImageDlg::OnBnClickedOption()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*m_strEvents = L"";
	UpdateData(FALSE);*/
	m_bEcho = !m_bEcho;
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_CLEAR);
	pEdit->SetWindowText(m_bEcho? L"No echo" : L"Echo");
}

HBRUSH CGetImageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_CLEAR || pWnd->GetDlgCtrlID() == IDC_ABOUT)
	{
		pDC->SetTextColor(RGB(0,0,255)); // ��ɫ����
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CGetImageDlg::OnStnClickedAbout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_DlgAbout.DoModal();
}

BOOL CGetImageDlg::SetTipText( UINT id, NMHDR *pTTTStruct, LRESULT *pResult )
{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
		UINT nID = pTTTStruct->idFrom;   //�õ���Ӧ����ID���п�����HWND   

		//����nID�Ƿ�ΪHWND
		if (pTTT->uFlags & TTF_IDISHWND) // ID IS HWND
		{
			//��HWND�õ�IDֵ����Ȼ��Ҳ����ͨ��HWNDֵ���ж�
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