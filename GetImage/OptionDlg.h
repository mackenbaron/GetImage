#pragma once

#include "Resource.h"
#include "AdvDlg.h"

// COptionDlg dialog

class COptionDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionDlg();

// dialogdata
	enum { IDD = IDD_OPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	UINT	m_ucMaxImg;
	UINT	m_ucMaxPage;
	int		m_nMaxThread;
	//BOOL m_bNewFolder;
	//int m_nRename;
	BOOL	m_bRelatvImg;
	BOOL	m_bRelatvPage;
	//BOOL m_bNoRptImg;
	//BOOL m_bNoRptPage;
	//BOOL m_bRename;
	//int m_nRename;
	//int				m_nRename;
	CString			m_strImgInc;
	CString			m_strPageInc;
	BOOL			m_bImgInc;
	BOOL			m_bPageInc;

	CAdvDlg			m_dlgAdv;
	//CToolTipCtrl	m_tt;
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//BOOL PreTranslateMessage(MSG *pMsg);
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedBtnAdv();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	UINT m_ucMaxLevel;
	CString m_strRename;
};
