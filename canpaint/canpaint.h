
// canpaint.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcanpaintApp:
// �йش����ʵ�֣������ canpaint.cpp
//

class CcanpaintApp : public CWinApp
{
public:
	CcanpaintApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcanpaintApp theApp;