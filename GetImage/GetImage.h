
// GetImage.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CGetImageApp:
// 有关此类的implement，请参阅 GetImage.cpp
//

class CGetImageApp : public CWinApp
{
public:
	CGetImageApp();

// 重写
	public:
	virtual BOOL InitInstance();

// implement

	DECLARE_MESSAGE_MAP()
};

extern CGetImageApp theApp;