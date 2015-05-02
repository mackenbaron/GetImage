#include "stdafx.h"
#include "GetImageDlg.h"

void CGetImageDlg::SetStatus( LPCTSTR str/*, int nCode*/ )
{
	CWnd *p = GetDlgItem(IDC_STATUS);
	p->SetWindowText(str);
}

void CGetImageDlg::Echo( LPCTSTR str )
{
	if (m_bEcho)
	{
		m_strEvents += str;
		m_strEvents += "\r\n";
		CEdit *p = (CEdit *)GetDlgItem(IDC_EVENTS);
		p->SetWindowText(m_strEvents);

		int nNow = p->GetFirstVisibleLine(),
			nLast = p->GetLineCount();
		p->LineScroll(nLast-nNow);

		int nMin,nMax;
		p->GetScrollRange(SB_VERT,&nMin,&nMax);
		p->SetScrollPos(SB_VERT,nMax);
	}
}

void CGetImageDlg::InsertAddress(CComboBoxEx *pCB,CString strAddress)
{
	if (strAddress.GetLength() < 9) return;
	if (strAddress.Left(7) != L"http://") strAddress = L"http://"+strAddress;
	::SlashUrl(strAddress);

	int nCnt = pCB->GetCount();
	CString strTemp;
	int i;
	for (i = 0;i < nCnt; i++)
	{
		pCB->GetLBText(i,strTemp);
		if(strAddress == strTemp) break;
	}
	if (i == nCnt)
	{
		COMBOBOXEXITEM item;
		item.mask = CBEIF_TEXT;
		item.iItem = -1;
		item.pszText = (LPTSTR)(LPCTSTR)strAddress;
		pCB->InsertItem(&item);
	}
}

void CGetImageDlg::SelectAll()
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EVENTS);
	pEdit->SetFocus();
	pEdit->SetSel(0,-1);
}

//CTime	 m_time;
DWORD g_tick;
void tic()
{
	g_tick = GetTickCount();
	/*m_time = CTime::GetCurrentTime();
	Echo(m_time.Format(L"%c"));*/
}

CString toc()
{
	/*m_time = CTime::GetCurrentTime()-m_time;
	Echo(m_time.Format(L"delta: %c"));*/
	g_tick = GetTickCount()-g_tick;
	CString tmp;
	tmp.Format(L"time used:%u",g_tick);
	return tmp;
}
