#pragma once


// CAdvDlg dialog

class CAdvDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvDlg)

public:
	CAdvDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdvDlg();

// dialog data
	enum { IDD = IDD_ADV_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bUseImg;
	CString m_strImg;
	BOOL m_bUsePage;
	CString m_strPage;
	int m_nImgScope;
	CString m_strTag;
	CString m_strAtt;
	CString m_strExt;
	int m_nPageScope;
};
