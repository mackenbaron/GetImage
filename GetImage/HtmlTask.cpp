#include "stdafx.h"
#include "GetImageDlg.h"


int HtmlGetTag( CString &strHtml, CString strTagName, int nFrom, CString &strTagContent )
{
	if (strHtml.IsEmpty() || strTagName.IsEmpty())
	{
		return -1;
	}

	 // ���Ǳ�ǩ�Ĵ�Сд���������Ǵ�С���
	int idx = strHtml.Find(L"<"+strTagName.MakeUpper()+L" ",nFrom),
		idx1 = strHtml.Find(L"<"+strTagName.MakeLower()+L" ",nFrom);
	if (idx > -1 && idx1 > -1)
		idx = __min(idx,idx1);
	else
		idx = __max(idx,idx1);

	if (idx == -1)
	{
		return -1;
	}
	
	idx1 = strHtml.Find(L">",idx);
	if (idx1 == -1)
	{
		return -1;
	}

	strTagContent = strHtml.Mid(idx+1,idx1-idx-1);
	if (strTagContent.Right(1) == L"/") // ȥ�� > ǰ��� / (����еĻ�)
		strTagContent = strTagContent.Left(strTagContent.GetLength()-1);
	return idx+1;
}

int HtmlTagToAttrib( CString &strTag, CString strAttName, CString &strAttContent )
{
	if (strTag.IsEmpty() || strAttName.IsEmpty())
	{
		return -1;
	}
	int idx = strTag.Find(L" "+strAttName.MakeUpper()+L"="), // �������ԵĴ�Сд���������Ǵ�С���
		idx1 = strTag.Find(L" "+strAttName.MakeLower()+L"=");
	if (idx > -1 && idx1 > -1)
		idx = __min(idx,idx1);
	else
		idx = __max(idx,idx1);

	if (idx == -1)
	{
		return -1;
	}

	idx1 = strTag.Find(L" ",idx+1);
	if (idx1 == -1)
	{
		idx1 = strTag.GetLength();
	}

	idx += strAttName.GetLength()+2;
	strAttContent = strTag.Mid(idx,idx1-idx);
	if (strAttContent.Left(1) == L"\"" || strAttContent.Left(1) == L"'") 
		strAttContent = strAttContent.Right(strAttContent.GetLength()-1);
	while (strAttContent.Right(1) == L"\"" || strAttContent.Right(1) == L"'" || strAttContent.Right(1) == L"\n") 
		strAttContent = strAttContent.Left(strAttContent.GetLength()-1);
	//strAttContent.MakeLower();
	return idx;
}


void CGetImageDlg::GetImgList(CString &strPage)
{
	int idx = 0,idx1,idx2;
	CString strTag,strUrl,strExt;
	CString status;
	status.LoadString(IDS_STATUS_ANLZ_IMG); SetStatus(status);

	while ((idx1 = ::HtmlGetTag(m_strCurPageCtt,m_optDlg.m_dlgAdv.m_strTag,idx,strTag)) != -1)
	{
		if ((idx2 = ::HtmlTagToAttrib(strTag,m_optDlg.m_dlgAdv.m_strAtt,strUrl)) != -1)
		{
			if (strUrl.IsEmpty()) 
			{
				idx = idx1+strTag.GetLength();
				continue;
			}
			if (m_optDlg.m_bRelatvImg)
			{
				::CompleteRelativeUrl(strPage,strUrl);
			}

			if (m_lstImg.Find(strUrl) != NULL) 
			{
				idx = idx1+strTag.GetLength();
				continue; 
			}
			// ��ֹһ����ҳ�е��ظ�ͼƬ��֮���Բ������������ع���ͼƬ����data�������������������ͼƬ��
			// ʵ�����ص�ͼƬ�ܿ��ܱȼ�¼�����٣�������������Ļ��ܿ������ظ�ͼƬ��

			//if (strUrl.Left(7) == L"http://" &&
			//	strUrl.Find(L"\"") == -1	 && // �������ŵ�ͼƬ����ͨ������ҳ�������
			//	strUrl.Find(L"'") == -1		 &&
			//	(!m_optDlg.m_bImgInc		 || /*strUrl.Find(m_optDlg.m_strImgInc) == -1*/
			//	  MyCheckURL(strUrl)))
			if (MyCheckUrl(strUrl,IDC_IMG_INC,idx1+idx2))
			{
				//URLINFO ui;
				m_lstImg.AddTail(strUrl);
				//m_lstImgStart.AddTail(idx1+idx2-m_nScope);
				//m_lstImgEnd.AddTail(idx1+idx2+m_nScope);
			}
		}
		idx = idx1+strTag.GetLength();
	}
}

void CGetImageDlg::RenewPageList(CString &strPage)
{
	int idx = 0,idx1,idx2;
	CString strTag,strUrl;
	CString status;
	status.LoadString(IDS_STATUS_ANLZ_PAGE); SetStatus(status);

	while ((idx1 = ::HtmlGetTag(m_strCurPageCtt,L"a",idx,strTag)) != -1)
	{
		if ((idx2 = ::HtmlTagToAttrib(strTag,L"href",strUrl)) != -1)
		{
			if (m_optDlg.m_bRelatvPage)
				::CompleteRelativeUrl(strPage,strUrl);

			//if (strUrl.Left(7) == L"http://"			&&
			//	m_lstPage.GetCount() < MAX_PAGE_LIST	&&	// �����б��ȣ���ֹ����
			//	/*!m_lstUsedPage.Find(strUrl)				&&  // ȥ�����ع�������*/ // DownloadPageTask implement��
			//	!(m_optDlg.m_bPageInc && strUrl.Find(m_optDlg.m_strPageInc) == -1))
			if (MyCheckUrl(strUrl,IDC_PAGE_INC,idx1+idx2))
			{
				::SlashUrl(strUrl);
				m_lstPage.AddTail(strUrl);
				//m_lstPageStart.AddTail(idx1+idx2-m_nScope);
				//m_lstPageEnd.AddTail(idx1+idx2+m_nScope);
			}
		}

		idx = idx1+strTag.GetLength();
	}
}

void CompleteRelativeUrl( CString strPage,CString &strUrl )
{
	if (strUrl.Left(1) == L"/") // ��Ŀ¼��־
	{
		int idx = strPage.Find(L"/",7);
		//if (idx != -1)
			strPage = strPage.Left(idx);
		strUrl = strPage+strUrl;
	}

	else if (strUrl.Left(3) == L"../") // �ϼ�Ŀ¼��־
	{
		int idx = strPage.ReverseFind(L'/');
		strPage = strPage.Left(idx);
		while(strUrl.Left(3) == L"../")
		{
			strUrl = strUrl.Right(strUrl.GetLength()-3);
			idx = strPage.ReverseFind(L'/');
			if (idx != -1) strPage = strPage.Left(idx);
		}
		strUrl = strPage+L"/"+strUrl;
	}

	else if (strUrl.Left(4) != L"http") // is not http or https. �������·������ src="img/1.jpg"
	{
		int idx = strPage.ReverseFind(L'/');
		strUrl = strPage.Left(idx+1)+strUrl;
	}
	
}

void SlashUrl(CString &url) // ���ʵ��������urlĩβ���� /
{
	if (url.Right(1) == L"/") return;
	if (url.Find(L"/",7) == -1) url += L"/"; // �� http://www.sina.com.cn�� http:// ������ /
	else 
	{
		int idx = url.ReverseFind(L'/');
		if (url.Find(L".",idx+1) == -1 && 
			url.Find(L"?",idx+1) == -1) // ���һ�� / ��û�� . �� ? ��˵�����һ�� / ����һ���ļ��������Լ��ܽ�ģ�
			url += L"/";
	}
}

bool MyCheckEachUrl(CString &str, STRLST &lstAnd,STRLST &lstOr,STRLST &lstNot)
{
	bool ret1 = true;
	POSITION pos = lstAnd.GetHeadPosition();
	for (int i = 0; i < lstAnd.GetCount(); i++)
	{
		CString &item = lstAnd.GetNext(pos);
		ret1 = (ret1 && (str.Find(item) != -1));
	}

	bool ret2 = (lstOr.GetCount() == 0);
	pos = lstOr.GetHeadPosition();
	for (int i = 0; i < lstOr.GetCount(); i++)
	{
		CString &item = lstOr.GetNext(pos);
		ret2 = (ret2 || (str.Find(item) != -1));
	}

	bool ret3 = true;
	pos = lstNot.GetHeadPosition();
	for (int i = 0; i < lstNot.GetCount(); i++)
	{
		CString &item = lstNot.GetNext(pos);
		ret3 = (ret3 && (str.Find(item) == -1));
	}

	return (ret1 && ret2 && ret3);
}

bool CGetImageDlg::MyCheckUrl(CString url, int nType, int nPos)
{
	if (url.Left(4) != L"http"	||
		url.Find(L"\"") != -1		||
		url.Find(L"'") != -1) // ������ͼƬ���ӻ�����ҳ���Ӷ�Ҫ����
		return false;
	if (nType == IDC_IMG_INC) // ������ԴID������
	{
		if (m_optDlg.m_bImgInc)
		{
			return (url.Find(m_strImgInc) != -1);
		}
		else if (m_optDlg.m_dlgAdv.m_bUseImg)
		{
			CString strContext = m_strCurPageCtt.Mid(nPos-m_nImgScope,url.GetLength()+m_nImgScope*2)+url;
			strContext.MakeLower();
			return MyCheckEachUrl(strContext,m_lstImgAnd,m_lstImgOr,m_lstImgNot);
		}
		return true;
	}
	else if (nType == IDC_PAGE_INC)
	{
		if (m_lstPage.GetCount() >= MAX_PAGE_LIST)
			return false;
		if (m_optDlg.m_bPageInc)
		{
			return (url.Find(m_strPageInc) != -1);
		}
		else if (m_optDlg.m_dlgAdv.m_bUsePage)
		{
			CString strContext = m_strCurPageCtt.Mid(nPos-m_nPageScope,url.GetLength()+m_nPageScope*2)+url;
			strContext.MakeLower();
			return MyCheckEachUrl(strContext,m_lstPageAnd,m_lstPageOr,m_lstPageNot);
		}
		return true;
	}
}

TCHAR* MyGetElem(LPTSTR str, TCHAR *ret)
{
	TCHAR *idx;
	int len;
	if (*str != L'"')
	{
		idx = wcschr(str,L' ');
		len = idx-str;
		wcsncpy(ret,str,len);
		*(ret+len) = L'\0';
	}
	else
	{
		idx = wcschr(str+1,L'"');
		if (!idx) idx = str+wcslen(str)-1;
		len = idx-str;
		wcsncpy(ret,str+1,len-1);
		*(ret+len-1) = L'\0';
		if (idx != str+wcslen(str)-1) idx++;
	}
	return idx;
}

void CGetImageDlg::MyAddToList(LPCTSTR key, STRLST &lstAnd,STRLST &lstOr, STRLST &lstNot)
{
	lstAnd.RemoveAll();lstOr.RemoveAll();lstNot.RemoveAll();
	TCHAR *tmp = new TCHAR[wcslen(key)];
	while(wcslen(key) > 1)
	{
		switch(*(key+1))
		{
		case L'+':
			key = MyGetElem((LPTSTR)key+2,tmp);
			lstAnd.AddTail(wcslwr(tmp));
			break;
		case L'-':
			key = MyGetElem((LPTSTR)key+2,tmp);
			lstNot.AddTail(wcslwr(tmp));
			break;
		default:
			key = MyGetElem((LPTSTR)key+1,tmp);
			if (wcslen(tmp) > 0) lstOr.AddTail(wcslwr(tmp));
		}
	}
	delete tmp;
}