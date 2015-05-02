
// GetImageDlg.h : 头文件
//

#pragma once

#include "OptionDlg.h"
#include "AboutDlg.h"
#include "afxcmn.h"

#define MAX_FOLDER		20	// 文件夹名长度
#define MAX_PAGE_LIST	1000 // 网页列表长度
#define MAX_TRY_IMG		3 // 下载图片不成功时重试次数
#define MAX_TRY_PAGE	2
#define SLEEP_TIME		500 // 重试前的等待时间

typedef CList<CString,CString> STRLST;

struct URLINFO // obsolete
{
	CString url;
	int nStart; // url在m_strCurPageCtt的起止位置
	int nEnd;
	int nLevel; // 主页算第一层，其子链接算第二层，以此类推
};

// CGetImageDlg dialog
class CGetImageDlg : public CDialog
{

	// user defined -->

public:

	UINT RunTask(); // 由StartTask调用，下载、分析网页和图片，返回结束码
	HRESULT DownloadSaveImg(CString &strUrl); // 由 DldImgProc 调用，下载存储图片并重命名
	UINT DownloadPageTask(); // 由 DldPageProc 调用

protected:

	bool		m_bStarted; // 是否已开始下载
	CString		m_strAddress; // ComboBox中的URL
	COptionDlg	m_optDlg;
	CAboutDlg	m_DlgAbout;
	HACCEL		m_hAccelTable; //快捷键
	CString		m_strEvents; // Echo 的字符串
	bool		m_bEcho; // 是否回显

	CString		m_strCurPageCtt, m_strBufPageCtt;
	CString		m_strBufUrl, m_strCurUrl;
	CString		m_strImgInc, m_strPageInc;
	CString		m_strFolder; // 存储下载的图片的文件夹
	//CList<URLINFO,URLINFO> m_lstImg, m_lstPage;
	STRLST		m_lstImg, m_lstPage, m_lstUsedPage;
	int			m_nDldImg, m_nDldPage; // 已下载的图片数和网页数

	CInternetSession m_session; // 网络连接相关
	//CHttpConnection *m_pConnect;
	//CHttpFile		*m_pFile;

	CCriticalSection	m_csThrdNum, m_csBufCtt; // 线程同步相关
	int					m_nThrdNum;
	CEvent				m_event;
	CProgressCtrl		m_pcThrdNum;

	STRLST	m_lstImgAnd,m_lstImgOr,m_lstImgNot; // url过滤相关
	STRLST	m_lstPageAnd,m_lstPageOr,m_lstPageNot;
	//CList<int>	m_lstImgStart,m_lstImgEnd;
	//CList<int>	m_lstPageStart,m_lstPageEnd,m_lstPageLevel;
	int		m_nImgScope,m_nPageScope;

	void OnStop(); // 结束下载
	void StartTask(); // 由 OnBtnClickedOK 调用，初始化变量，新建文件夹

	void InsertAddress(CComboBoxEx *pCB, CString strAddress); // 在不重复的前提下向ComboBox中插入输入的URL
	void SetStatus(LPCTSTR str/*, int nCode*/); // 写状态栏
	void Echo(LPCTSTR str); // 回显信息
	
	DWORD DownloadPage(CString strUrl); // 下载html到 m_strCurPageContent，返回状态码
	void GetImgList(CString &strPage); // 从 m_strCurPageContent 中分析出 m_imgList
	void RenewPageList(CString &strPage); // 从 m_strCurPageContent 中分析网页链接，插入 m_pageList 尾
	bool MyCheckUrl(CString url, int nType, int nPos); // 更新图片/网页列表时检查取得的url是否符合要求
	void MyAddToList(LPCTSTR key, STRLST &lstAnd, STRLST &lstOr, STRLST &lstNot); // 解析用户在高级选项中输入的关键字

	// <-- user defined

	// system defined & message handler -->

// 构造
public:
	CGetImageDlg(CWnd* pParent = NULL);	// standard constructor


// dialogdata
	enum { IDD = IDD_GETIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// implement
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnSize(UINT nType, int cx, int cy); // 改变窗口大小时，控件大小位置随之改变
	afx_msg void OnBnClickedOk(); // 点击“开始/停止”按钮时，或在ComboBox中敲回车时
	afx_msg void OnCbnSelendokAddress(); // 从ComboBox中选择一条URL时
	afx_msg void OnBnClickedOption();
	afx_msg void SelectAll();
	void OnCancel();
	afx_msg void OnBnClickedClear();
	afx_msg void OnStnClickedAbout();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // 改变字体颜色
	BOOL SetTipText(UINT id, NMHDR *pTTTStruct, LRESULT *pResult); // 设置Tooltip
	// <-- system defined
};

// 从 strHtml 的 nFrom 个字符(start with 0)开始解析出第一个名为 strTagName 的tag内容，放入strTagContent，
// 不包括</>，包括TagName。返回值为tag的字符串起点，未找到返回-1
int HtmlGetTag(CString &strHtml, CString strTagName, int nFrom, CString &strTagContent);

// 从 strTag 中寻找名为 strAttName 的属性，返回属性内容(不含引号)和字符串起点，未找到返回-1
int HtmlTagToAttrib(CString &strTag, CString strAttName, CString &strAttContent);

// 补全相对链接strUrl，strPage为strUrl所在的网页的URL。能够将strUrl前面的 / 和 ../ 用适当的内容替换。
// 补全后的strUrl用 http:// 开头。不检查两个参数格式的对错
void CompleteRelativeUrl(CString strPage,CString &strUrl);

void SlashUrl(CString &url); // 在适当的情况下url末尾加上 /

// 开启新线程用于下载，防止下载过程中界面无法操作
UINT ThreadProc(LPVOID pParam);

// 下载图片线程
UINT DldImgProc(LPVOID pParam);

// 下载网页线程
UINT DldPageProc(LPVOID pParam);

extern DWORD g_tick;
void tic();
CString toc(); // 用于记时，起点处用tic，终点处用toc返回毫秒数

extern CGetImageDlg *pDlg;
extern int cnt;