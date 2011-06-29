
// DBUpdate.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDBUpdateApp:
// See DBUpdate.cpp for the implementation of this class
//

class CDatabaseEx;

class CDBUpdateApp : public CWinApp
{
public:
	CDBUpdateApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
public:
	CDatabaseEx* m_pDatabase;
};

extern CDBUpdateApp theApp;