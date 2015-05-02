#include "stdafx.h"
#include "GetImageDlg.h"
#include <Urlmon.h> // �ں� URLDownloadToFile ������

UINT ThreadProc(LPVOID pParam)
{
	CGetImageDlg *pDlg = (CGetImageDlg *)pParam;
	return pDlg->RunTask();
}


UINT DldImgProc( LPVOID pParam )
{
	CString strUrl = (LPCTSTR)pParam;
	return pDlg->DownloadSaveImg(strUrl);
}


UINT DldPageProc( LPVOID pParam )
{
	//CString strUrl = (LPCTSTR)pParam;
	return pDlg->DownloadPageTask();
}

UINT CGetImageDlg::DownloadPageTask()
{
	m_csBufCtt.Lock();
	CString strUrl;
	do 
	{
		do 
		{
			if (m_lstPage.IsEmpty()) // δ�ܳɹ�������һ����ҳ
			{
				m_csBufCtt.Unlock();
				return 1;
			}
			strUrl = m_lstPage.RemoveHead();
		} while (m_lstUsedPage.Find(strUrl));

		//Echo(L"\t\t��ʼ������ҳ��"+strUrl);
	} while (DownloadPage(strUrl));
	m_strBufUrl = strUrl;
	m_lstUsedPage.AddTail(strUrl);

	m_csBufCtt.Unlock();
	return 0;
}

DWORD CGetImageDlg::DownloadPage( CString strUrl )
{
	m_strBufPageCtt.Empty();
	DWORD ret = 0;
	CHttpFile *pFile = NULL;

	//ret = ::URLDownloadToFile(NULL,strUrl,L"tmp.html",NULL,NULL);
	//if (ret)
	//{
	//	return ret;
	//}
	//else
	//{
	//	CStdioFile file(L"tmp.html",CFile::modeRead);
	//	/*CString line;
	//	while(file.ReadString(line))
	//		m_strBufPageCtt += line;*/
	//	static char line[2000];
	//	while(file.Read((LPTSTR)line,1000)) // ע��˴�1000���ַ�����
	//	{
	//		m_strBufPageCtt += line;
	//		//m_strBufPageCtt += "\r\n";
	//	}
	//}
	//// ����Ҳ����

	try
	{
		static CString strServer;
		static CString strObject;
		static INTERNET_PORT nPort;
		static DWORD dwServiceType;

		if(!AfxParseURL(strUrl,dwServiceType,strServer,strObject,nPort)||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			return HTTP_STATUS_NOT_FOUND; // ��ַ����
		}

		/*Connect = m_session.GetHttpConnection(strServer,nPort);
		File = Connect->OpenRequest(1,strObject);
		File->SendRequest();*/
		pFile = (CHttpFile *)m_session.OpenURL(strUrl); // ��OpenRequest���ܻ���deleteʱ������DownloadUrlToFile�»���д�����ϻ�ʱ��

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if (dwRet != HTTP_STATUS_OK) 
		{
			pFile->Close();
			//Connect->Close();
			if (pFile != NULL) delete pFile;
			//if (Connect != NULL) delete Connect;
			return dwRet; // ������ز��ɹ�
		}

		static char line[2000];
		//CString line; // ��CString Read����ȫ������
		while(pFile->ReadString((LPTSTR)line,1000)) // ע��˴�1000���ַ�����
		{
			m_strBufPageCtt += line;
			//m_strBufPageCtt += "\r\n";
			//TRACE(L"size:%d\r\n",m_strBufPageCtt.GetLength());
		}
		pFile->Close();
		//Connect->Close();
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[100];
		ret = pEx->m_dwError;
		pEx->GetErrorMessage(szErr, 100);
		CString err;
		err.LoadString(IDS_DLD_PAGE_ERR);
		err += szErr;
		err = err.Left(err.GetLength()-1); // ȥ���س���
		Echo(err);
		pEx->Delete();
	}

	if (pFile != NULL) delete pFile;
	//if (Connect != NULL) delete Connect;

	return ret;
}

int cnt = 0;

HRESULT CGetImageDlg::DownloadSaveImg( CString &strUrl ) //����ͬ���ļ��򸲸�
{
	m_csThrdNum.Lock();
	m_nThrdNum++;
	cnt++;
	m_csThrdNum.Unlock();

	CString strUrl1 = strUrl,strExt,strQry;
	int idxQ = strUrl1.ReverseFind('?');
	if (idxQ != -1) // ȥ��ͼƬ�����query
	{
		strQry = strUrl1.Right(strUrl1.GetLength()-idxQ);
		strUrl1 = strUrl1.Left(idxQ);
	}

	int len1 = strUrl1.GetLength();
	int idxD = strUrl1.Find(L".",len1-5);
	if (strUrl1.Right(1) == L"/" || idxD == -1) // ȷ����չ��
		strExt = L'.'+m_optDlg.m_dlgAdv.m_strExt;
	else 
	{
		strExt = strUrl1.Right(len1-idxD); // ����'.'
		strUrl1 = strUrl1.Left(idxD);
	}

	int idxS = strUrl1.ReverseFind('/'),
		idxHead = strUrl1.Find(L"://");
	CString pic = strUrl1.Mid(idxS+1)+strQry,
			web = strUrl1.Mid(idxHead+3)+strQry;
	CString index;
	index.Format(L"%d",cnt);

	CString name = m_optDlg.m_strRename;
	name.Replace(L"%w",web);
	name.Replace(L"%n",pic);
	name.Replace(L"%i",index);
	if (name.IsEmpty()) name = web;
	name += strExt;

	name.Replace(L'/',L'+');
	name.Replace(L'\\',L'+');
	name.Replace(L':',L'+');
	name.Replace(L'?',L'+');
	name.Replace(L'|',L'+');


	/*CString strName;
	int idx, idxExt = strUrl.Find('.',strUrl.GetLength()-5);
	switch (m_optDlg.m_nRename)
	{
	case 0:
		idx = strUrl.ReverseFind('/');
		strName = strUrl.Right(strUrl.GetLength()-idx-1);
		strName.Replace(L'?',L'+');
		strName.Replace(L'|',L'+');
		if (idxExt == -1) strName += L".jpg";
		break;

	case 1:
		strName.Format(L"%d",m_nDldImg+1);
		if (idxExt == -1) strName += L".jpg";
		else strName += strUrl.Right(strUrl.GetLength()-idxExt); // ��չ��
		break;

	case 2:
		strName = strUrl.Right(strUrl.GetLength()-7);
		strName.Replace(L'/',L'+');
		strName.Replace(L':',L'+');f
		strName.Replace(L'?',L'+');
		strName.Replace(L'|',L'+');
		if (idxExt == -1) strName += L".jpg";
	}*/

	HRESULT ret;
	for (int i = 0;i < MAX_TRY_IMG; i++)
	{
		ret = ::URLDownloadToFile(NULL, strUrl, m_strFolder+name, NULL,NULL);
		if (!ret) break;
		Sleep(SLEEP_TIME);
	}
	/*HRESULT ret = ::URLDownloadToFile(NULL, L"http://fmn029.xnimg.cn/fmn029/pic001/20090212/14/35/large_RU3t_19248p206107.jpg",
		L"1.jpg", NULL,NULL);*/

	if (ret) 
	{
		CString status,tmp;
		tmp.LoadString(IDS_DLD_IMG_ERR);
		status.Format(tmp,ret,strUrl);
		Echo(status);
	}

	m_csThrdNum.Lock();
	m_nThrdNum--;
	m_csThrdNum.Unlock();
	m_event.SetEvent();
	return ret;
}