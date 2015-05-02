// OptionDlg.cpp : implement�ļ�
//

#include "stdafx.h"
#include "GetImage.h"
#include "OptionDlg.h"


// COptionDlg dialog

IMPLEMENT_DYNAMIC(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
	, m_ucMaxImg(100)
	, m_ucMaxPage(0)
	, m_nMaxThread(3)
	//, m_bNewFolder(TRUE)
	//, m_nRename(2)
	, m_bRelatvImg(TRUE)
	, m_bRelatvPage(TRUE)
	//, m_bNoRptImg(TRUE)
	//, m_bNoRptPage(FALSE)
	//, m_bRename(FALSE)
	//, m_nRename(0)
	//, m_nRename(2)
	, m_strImgInc(_T(""))
	, m_strPageInc(_T(""))
	, m_bImgInc(FALSE)
	, m_bPageInc(FALSE)
	, m_ucMaxLevel(0)
	, m_strRename(_T("%w"))
{

}

COptionDlg::~COptionDlg()
{
}

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//UpdateData(FALSE);
	CButton *p1 = (CButton *)GetDlgItem(IDC_IMG_INC),
		*p2 = (CButton *)GetDlgItem(IDC_IMG_TXT);
	CStatic *p = (CStatic *)GetDlgItem(IDC_STATIC1);

	if (m_dlgAdv.m_bUseImg)
	{
		p1->SetCheck(BST_UNCHECKED);
		p1->EnableWindow(FALSE);
		p2->EnableWindow(FALSE);
		p->EnableWindow(FALSE);
	}

	p1 = (CButton *)GetDlgItem(IDC_PAGE_INC);
	p2 = (CButton *)GetDlgItem(IDC_PAGE_TXT);
	//p = (CStatic *)GetDlgItem(IDC_STATIC2);

	if (m_dlgAdv.m_bUsePage)
	{
		p1->SetCheck(BST_UNCHECKED);
		p1->EnableWindow(FALSE);
		p2->EnableWindow(FALSE);
		p->EnableWindow(FALSE);
	}

	CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_THRDNUM);
	pSlider->SetRange(0,10,TRUE);
	//pSlider->SetPos(m_nMaxThread);
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMGNUM, m_ucMaxImg);
	DDX_Text(pDX, IDC_PAGENUM, m_ucMaxPage);
	DDX_Slider(pDX, IDC_THRDNUM, m_nMaxThread);
	//DDX_Radio(pDX, IDC_NO_RENAME, m_nRename);
	DDX_Check(pDX, IDC_ECHO_IMG, m_bRelatvImg);
	DDX_Check(pDX, IDC_ECHO_PAGE, m_bRelatvPage);
	//DDX_Check(pDX, IDC_NOREPEAT_IMG, m_bNoRptImg);
	//DDX_Check(pDX, IDC_NOREPEAT_PAGE, m_bNoRptPage);
	//DDX_Radio(pDX, IDC_NO_RENAME, m_bRename);
	//DDX_Radio(pDX, IDC_NO_RENAME, m_nRename);
	//DDX_Radio(pDX, IDC_RN_ORI, m_nRename);
	DDX_Text(pDX, IDC_IMG_TXT, m_strImgInc);
	DDX_Text(pDX, IDC_PAGE_TXT, m_strPageInc);
	DDX_Check(pDX, IDC_IMG_INC, m_bImgInc);
	DDX_Check(pDX, IDC_PAGE_INC, m_bPageInc);
	//DDX_Text(pDX, IDC_LEVELNUM, m_ucMaxLevel);
	DDX_Text(pDX, IDC_RENAME, m_strRename);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ADV, &COptionDlg::OnBnClickedBtnAdv)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// COptionDlg ��Ϣ�������

void COptionDlg::OnBnClickedBtnAdv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CWnd *pWnd = GetDlgItem(IDC_TXT_ADV);
	//pWnd->ShowWindow(SW_SHOW);
	//pWnd->SetWindowText(L"�߼�������һЩ�򵥵���ַ���ˣ���implementѡ��������ͼƬ��Ŀ�ġ�\n"
	//					L"    ����Ӧ�õ����������(),&&,|,!��Ӧ�õķ�ʽ���Ʊ�������е��߼����ʽ��\n"
	//					L"    ���� img | photo ��ʾ��ַ�к����ַ���img��photo��"
	//					L"large && ! simple ��ʾ��ַ��Ҫ����large�����ܺ���simple��\n"
	//					L"    ��������ȷ�����ȼ����������ַ�����ǰ��˳��ע���ַ����ͷ���֮��Ҫ�ÿո�ָ���");
	//pWnd = GetDlgItem(IDC_IMGNUM);
	//pWnd->EnableWindow(FALSE);
	//pWnd = GetDlgItem(IDC_PAGENUM);
	//pWnd->EnableWindow(FALSE);

	m_dlgAdv.DoModal();
	CButton *p1 = (CButton *)GetDlgItem(IDC_IMG_INC),
			*p2 = (CButton *)GetDlgItem(IDC_IMG_TXT);
	CStatic *p = (CStatic *)GetDlgItem(IDC_STATIC1);

	if (m_dlgAdv.m_bUseImg)
	{
		p1->SetCheck(BST_UNCHECKED);
		p1->EnableWindow(FALSE);
		p2->EnableWindow(FALSE);
	}
	else
	{
		p1->EnableWindow(TRUE);
		p2->EnableWindow(TRUE);
	}

	p1 = (CButton *)GetDlgItem(IDC_PAGE_INC);
	p2 = (CButton *)GetDlgItem(IDC_PAGE_TXT);
	//p = (CStatic *)GetDlgItem(IDC_STATIC2);

	if (m_dlgAdv.m_bUsePage)
	{
		p1->SetCheck(BST_UNCHECKED);
		p1->EnableWindow(FALSE);
		p2->EnableWindow(FALSE);
		p->EnableWindow(FALSE);
	}
	else
	{
		p1->EnableWindow(TRUE);
		p2->EnableWindow(TRUE);
		//p->EnableWindow(TRUE);
	}

	if (m_dlgAdv.m_bUseImg || m_dlgAdv.m_bUsePage)
		p->EnableWindow(TRUE);
	else 
		p->EnableWindow(FALSE);
}

void COptionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CWnd *pWnd = GetDlgItem(IDC_IMGNUM);
	//pWnd->EnableWindow(TRUE);
	//pWnd = GetDlgItem(IDC_PAGENUM);
	//pWnd->EnableWindow(TRUE);
	//pWnd = GetDlgItem(IDC_TXT_ADV);
	//pWnd->ShowWindow(SW_HIDE);
	//CDialog::OnLButtonUp(nFlags, point);
}
