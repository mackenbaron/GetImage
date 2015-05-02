
// GetImageDlg.h : ͷ�ļ�
//

#pragma once

#include "OptionDlg.h"
#include "AboutDlg.h"
#include "afxcmn.h"

#define MAX_FOLDER		20	// �ļ���������
#define MAX_PAGE_LIST	1000 // ��ҳ�б���
#define MAX_TRY_IMG		3 // ����ͼƬ���ɹ�ʱ���Դ���
#define MAX_TRY_PAGE	2
#define SLEEP_TIME		500 // ����ǰ�ĵȴ�ʱ��

typedef CList<CString,CString> STRLST;

struct URLINFO // obsolete
{
	CString url;
	int nStart; // url��m_strCurPageCtt����ֹλ��
	int nEnd;
	int nLevel; // ��ҳ���һ�㣬����������ڶ��㣬�Դ�����
};

// CGetImageDlg dialog
class CGetImageDlg : public CDialog
{

	// user defined -->

public:

	UINT RunTask(); // ��StartTask���ã����ء�������ҳ��ͼƬ�����ؽ�����
	HRESULT DownloadSaveImg(CString &strUrl); // �� DldImgProc ���ã����ش洢ͼƬ��������
	UINT DownloadPageTask(); // �� DldPageProc ����

protected:

	bool		m_bStarted; // �Ƿ��ѿ�ʼ����
	CString		m_strAddress; // ComboBox�е�URL
	COptionDlg	m_optDlg;
	CAboutDlg	m_DlgAbout;
	HACCEL		m_hAccelTable; //��ݼ�
	CString		m_strEvents; // Echo ���ַ���
	bool		m_bEcho; // �Ƿ����

	CString		m_strCurPageCtt, m_strBufPageCtt;
	CString		m_strBufUrl, m_strCurUrl;
	CString		m_strImgInc, m_strPageInc;
	CString		m_strFolder; // �洢���ص�ͼƬ���ļ���
	//CList<URLINFO,URLINFO> m_lstImg, m_lstPage;
	STRLST		m_lstImg, m_lstPage, m_lstUsedPage;
	int			m_nDldImg, m_nDldPage; // �����ص�ͼƬ������ҳ��

	CInternetSession m_session; // �����������
	//CHttpConnection *m_pConnect;
	//CHttpFile		*m_pFile;

	CCriticalSection	m_csThrdNum, m_csBufCtt; // �߳�ͬ�����
	int					m_nThrdNum;
	CEvent				m_event;
	CProgressCtrl		m_pcThrdNum;

	STRLST	m_lstImgAnd,m_lstImgOr,m_lstImgNot; // url�������
	STRLST	m_lstPageAnd,m_lstPageOr,m_lstPageNot;
	//CList<int>	m_lstImgStart,m_lstImgEnd;
	//CList<int>	m_lstPageStart,m_lstPageEnd,m_lstPageLevel;
	int		m_nImgScope,m_nPageScope;

	void OnStop(); // ��������
	void StartTask(); // �� OnBtnClickedOK ���ã���ʼ���������½��ļ���

	void InsertAddress(CComboBoxEx *pCB, CString strAddress); // �ڲ��ظ���ǰ������ComboBox�в��������URL
	void SetStatus(LPCTSTR str/*, int nCode*/); // д״̬��
	void Echo(LPCTSTR str); // ������Ϣ
	
	DWORD DownloadPage(CString strUrl); // ����html�� m_strCurPageContent������״̬��
	void GetImgList(CString &strPage); // �� m_strCurPageContent �з����� m_imgList
	void RenewPageList(CString &strPage); // �� m_strCurPageContent �з�����ҳ���ӣ����� m_pageList β
	bool MyCheckUrl(CString url, int nType, int nPos); // ����ͼƬ/��ҳ�б�ʱ���ȡ�õ�url�Ƿ����Ҫ��
	void MyAddToList(LPCTSTR key, STRLST &lstAnd, STRLST &lstOr, STRLST &lstNot); // �����û��ڸ߼�ѡ��������Ĺؼ���

	// <-- user defined

	// system defined & message handler -->

// ����
public:
	CGetImageDlg(CWnd* pParent = NULL);	// standard constructor


// dialogdata
	enum { IDD = IDD_GETIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// implement
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnSize(UINT nType, int cx, int cy); // �ı䴰�ڴ�Сʱ���ؼ���Сλ����֮�ı�
	afx_msg void OnBnClickedOk(); // �������ʼ/ֹͣ����ťʱ������ComboBox���ûس�ʱ
	afx_msg void OnCbnSelendokAddress(); // ��ComboBox��ѡ��һ��URLʱ
	afx_msg void OnBnClickedOption();
	afx_msg void SelectAll();
	void OnCancel();
	afx_msg void OnBnClickedClear();
	afx_msg void OnStnClickedAbout();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // �ı�������ɫ
	BOOL SetTipText(UINT id, NMHDR *pTTTStruct, LRESULT *pResult); // ����Tooltip
	// <-- system defined
};

// �� strHtml �� nFrom ���ַ�(start with 0)��ʼ��������һ����Ϊ strTagName ��tag���ݣ�����strTagContent��
// ������</>������TagName������ֵΪtag���ַ�����㣬δ�ҵ�����-1
int HtmlGetTag(CString &strHtml, CString strTagName, int nFrom, CString &strTagContent);

// �� strTag ��Ѱ����Ϊ strAttName �����ԣ�������������(��������)���ַ�����㣬δ�ҵ�����-1
int HtmlTagToAttrib(CString &strTag, CString strAttName, CString &strAttContent);

// ��ȫ�������strUrl��strPageΪstrUrl���ڵ���ҳ��URL���ܹ���strUrlǰ��� / �� ../ ���ʵ��������滻��
// ��ȫ���strUrl�� http:// ��ͷ�����������������ʽ�ĶԴ�
void CompleteRelativeUrl(CString strPage,CString &strUrl);

void SlashUrl(CString &url); // ���ʵ��������urlĩβ���� /

// �������߳��������أ���ֹ���ع����н����޷�����
UINT ThreadProc(LPVOID pParam);

// ����ͼƬ�߳�
UINT DldImgProc(LPVOID pParam);

// ������ҳ�߳�
UINT DldPageProc(LPVOID pParam);

extern DWORD g_tick;
void tic();
CString toc(); // ���ڼ�ʱ����㴦��tic���յ㴦��toc���غ�����

extern CGetImageDlg *pDlg;
extern int cnt;