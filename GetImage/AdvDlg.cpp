// AdvDlg.cpp : implement file
//

#include "stdafx.h"
#include "GetImage.h"
#include "AdvDlg.h"


// CAdvDlg dialog

IMPLEMENT_DYNAMIC(CAdvDlg, CDialog)

CAdvDlg::CAdvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvDlg::IDD, pParent)
	, m_bUseImg(FALSE)
	, m_strImg(_T(""))
	, m_bUsePage(FALSE)
	, m_strPage(_T(""))
	, m_nImgScope(500)
	, m_strTag(_T("img"))
	, m_strAtt(_T("src"))
	, m_strExt(_T("jpg"))
	, m_nPageScope(0)
{

}

CAdvDlg::~CAdvDlg()
{
}

void CAdvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_USEIMG, m_bUseImg);
	DDX_Text(pDX, IDC_IMGURL, m_strImg);
	DDX_Check(pDX, IDC_USEPAGE, m_bUsePage);
	DDX_Text(pDX, IDC_PAGEURL, m_strPage);
	DDX_Text(pDX, IDC_SCOPE, m_nImgScope);
	DDX_Text(pDX, IDC_EDIT1, m_strTag);
	DDX_Text(pDX, IDC_EDIT2, m_strAtt);
	DDX_Text(pDX, IDC_EDIT3, m_strExt);
	DDX_Text(pDX, IDC_SCOPE2, m_nPageScope);
}


BEGIN_MESSAGE_MAP(CAdvDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAdvDlg::OnInitDialog()
{
	CWnd *pWnd = GetDlgItem(IDC_TXT_ADV);
	CString str = L"Advanced URL filter:\n\n"
				  L"  Search images as if you're using Google.\n\n"
				  L"  Input one or more keyword,"
				  L"add '+' as prefix means the word have to appear,"
				  L"add '-' means the word mustn't appear,"
				  L"the text inside the quotation marks will be considered as a whole.\n\n"
				  L"  For example,\n    girl lady +pretty -\"just kidding\"\n"
				  L"means 'girl' or 'lady' have to appear in the search radius,"
				  L",pretty have to appear while 'just kidding' mustn't appear.";
	pWnd->SetWindowText(str);
	UpdateData(FALSE); // updtate data for the edit controls
	return TRUE;
}

// CAdvDlg 消息处理程序
