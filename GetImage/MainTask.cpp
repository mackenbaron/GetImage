#include "stdafx.h"
#include "GetImageDlg.h"
#include <atlconv.h>
//#include <atlrx.h>

void CGetImageDlg::StartTask()
{
	m_nDldImg = m_nDldPage = 0;
	m_lstImg.RemoveAll();
	m_lstPage.RemoveAll();
	m_lstUsedPage.RemoveAll();
	//m_lstImgStart.RemoveAll();
	//m_lstImgEnd.RemoveAll();
	//m_lstPageStart.RemoveAll();
	//m_lstPageEnd.RemoveAll();
	//m_lstPageLevel.RemoveAll(); // 暂不support分层

	m_nImgScope = m_optDlg.m_dlgAdv.m_nImgScope;
	m_nPageScope = m_optDlg.m_dlgAdv.m_nPageScope;

	if (m_optDlg.m_dlgAdv.m_bUseImg)
	{
		CString str = L' '+m_optDlg.m_dlgAdv.m_strImg+L' ';
		MyAddToList((LPCTSTR)str,m_lstImgAnd,m_lstImgOr,m_lstImgNot);
	}
	
	if (m_optDlg.m_dlgAdv.m_bUsePage)
	{
		CString str = L' '+m_optDlg.m_dlgAdv.m_strPage+L' ';
		/*MyAddToList(str,m_lstPageAnd,L'+');
		MyAddToList(str,m_lstPageOr,L'|');
		MyAddToList(str,m_lstPageNot,L'-');*/
		MyAddToList((LPCTSTR)str,m_lstPageAnd,m_lstPageOr,m_lstPageNot);
	}

	m_nThrdNum = 1;
	cnt = 0;
	//m_strAddress.MakeLower();

	UpdateData();

	/*if (m_strAddress.Left(7) != L"http://") // 在 InsertAddress 里implement了
	{
		m_strAddress = L"http://"+m_strAddress;
	}*/
	/*if (m_strAddress.Right(1) == L"/")
	{
		m_strAddress = m_strAddress.Left(m_strAddress.GetLength()-1);
	}*/
	//m_lstPage.AddHead(m_strAddress);


	m_strFolder = m_strAddress.Right(m_strAddress.GetLength()-7); // 去掉 http://
	int slash = m_strFolder.FindOneOf(L"/\\"); // 不能有"/"和"\"
	if (slash != -1)
	{
		m_strFolder = m_strFolder.Left(slash);
	}
	m_strFolder = m_strFolder.Left(MAX_FOLDER);
	::CreateDirectory((LPCTSTR)m_strFolder,NULL); // 若文件夹已存在则不作处理
	m_strFolder += L"/";
	
	if (m_optDlg.m_bImgInc && m_strImgInc.IsEmpty())
		m_strImgInc = m_strAddress;
	if (m_optDlg.m_bPageInc && m_strPageInc.IsEmpty())
		m_strPageInc = m_strAddress;

	::AfxBeginThread(ThreadProc,this,THREAD_PRIORITY_HIGHEST);
	//RunTask();
}

UINT CGetImageDlg::RunTask()
{
	int ret;
	CString status,tmp;

	status.LoadString(IDS_STATUS_DLD_PAGE); SetStatus(status+m_strAddress);
	if(ret = DownloadPage(m_strAddress))
	{
		tmp.LoadString(IDS_DLD_MPAGE_ERR);
		status.Format(tmp,ret);
		Echo(status);
	}
	else
	{
		m_strBufUrl = m_strAddress;
		m_lstUsedPage.AddTail(m_strAddress);
	}

	do
	{

// 分析网页链接
		m_pcThrdNum.SetPos(m_nThrdNum);
		status.LoadString(IDS_STATUS_DLD_PAGE); SetStatus(status);
		m_csBufCtt.Lock();
		if (m_strBufUrl.IsEmpty()) // 没有网页可供继续下载
		{
			m_csBufCtt.Unlock();
			break;
		}
		m_strCurPageCtt = m_strBufPageCtt;
		RenewPageList(m_strBufUrl);
		int nPage = m_lstPage.GetCount();
		m_strCurUrl = m_strBufUrl;
		m_strBufUrl.Empty();	
		m_csBufCtt.Unlock();


// 下载网页
		if (!m_bStarted) break;

		if (!m_lstPage.IsEmpty())
		{
			::AfxBeginThread(DldPageProc, NULL);
		}
		

// 分析图片链接
		tmp.LoadString(IDS_DLDD_PAGE);
		status.Format(tmp,++m_nDldPage,m_strCurUrl);
		Echo(status);
		GetImgList(m_strCurUrl);		
		tmp.LoadString(IDS_ANLZ_RSLT);
		status.Format(tmp, m_lstImg.GetCount(),nPage);
		Echo(status);
		if (!m_bStarted) break;


// 下载图片
		status.LoadString(IDS_STATUS_DLD_IMGS);
		SetStatus(status+m_strCurUrl);
		while(!m_lstImg.IsEmpty())
		{
			//Echo(m_lstImg.GetNext(pos));
			CString imgUrl = m_lstImg.RemoveHead();
			
			if (m_optDlg.m_nMaxThread == 0) // 单线程
			{
				tmp.LoadString(IDS_DLD_IMG);
				status.Format(tmp,++m_nDldImg,imgUrl);
				Echo(status);
				ret = DownloadSaveImg(imgUrl);
			}
			else // 多线程
			{
				for (int i = 0;i < m_nThrdNum-m_optDlg.m_nMaxThread+1;i++) // 线程数限制
				{
					::WaitForSingleObject(m_event.m_hObject,INFINITE);
				}
				::AfxBeginThread(DldImgProc,(LPVOID)(LPCTSTR)imgUrl);

				m_pcThrdNum.SetPos(m_nThrdNum);
				tmp.LoadString(IDS_DLD_IMG);
				status.Format(tmp,++m_nDldImg,imgUrl);
				Echo(status);
			}

			if (!m_bStarted) // || (m_nDldImg == m_optDlg.m_ucMaxImg && m_optDlg.m_ucMaxImg != 0))
				break;
			tmp.LoadString(IDS_STATUS_DLDD_NUM);
			status.Format(tmp,m_nDldImg);
			SetStatus(status);
		}

		if (!m_bStarted || (m_nDldImg >= m_optDlg.m_ucMaxImg && m_optDlg.m_ucMaxImg != 0)
			|| m_nDldPage == m_optDlg.m_ucMaxPage) // 检查结束条件
		{
			break;
		}


	}
	while(1);

	OnStop(); // 下载结束
	return 0;
}

void CGetImageDlg::OnStop()
{
	m_bStarted = false;
	CButton *pBtn = (CButton *)GetDlgItem(IDOK);
	pBtn->SetWindowText(L"Start");

	m_pcThrdNum.SetPos(0);
	CString tmp;
	tmp.LoadString(IDS_STOP); Echo(tmp);
	tmp.LoadString(IDS_STATUS_READY); SetStatus(tmp);
}
