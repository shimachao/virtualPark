
// virtualPark.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CvirtualParkApp: 
// �йش����ʵ�֣������ virtualPark.cpp
//

class CvirtualParkApp : public CWinApp
{
public:
	CvirtualParkApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CvirtualParkApp theApp;