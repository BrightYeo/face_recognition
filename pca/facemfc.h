// facemfc.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CfacemfcApp:
// �� Ŭ������ ������ ���ؼ��� facemfc.cpp�� �����Ͻʽÿ�.
//

class CfacemfcApp : public CWinApp
{
public:
	CfacemfcApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CfacemfcApp theApp;