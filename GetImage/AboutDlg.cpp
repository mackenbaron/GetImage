#include "stdafx.h"
#include "AboutDlg.h"
#include "GetImage.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_W, &CAboutDlg::OnNMClickSyslinkW)
END_MESSAGE_MAP()

void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult) // ����ҳ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	ShellExecute(NULL, L"open", L"http://yanke23.tk", NULL, NULL, SW_SHOWNORMAL);
	SetFocus(); // ȥ�������ϵ����߿�
}

void CAboutDlg::OnNMClickSyslinkW(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	ShellExecute(NULL, L"open", L"http://blog.csdn.net/viggin/archive/2009/09/12/4546900.aspx", NULL, NULL, SW_SHOWNORMAL);
	SetFocus();
}
