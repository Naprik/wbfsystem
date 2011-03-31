
// YDVaultMain.h : main header file for the YDVaultMain application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#define DB_ORACLE
// CYDVaultMainApp:
// See YDVaultMain.cpp for the implementation of this class
//
class CDatabaseEx;
class CFtpRef;
class CYDVaultMainApp : public CWinAppEx
{
public:
	CYDVaultMainApp();
	virtual ~CYDVaultMainApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnSysUsermrg();
	afx_msg void OnSysUserModifyPwd();
	afx_msg void OnSysRelogin();
	DECLARE_MESSAGE_MAP()
public:
	CMultiDocTemplate*  m_pDocTemplate;
	CDatabaseEx* m_pDatabase;
	CFtpRef*	 m_pFtpRef;
	virtual int ExitInstance();
};

extern CYDVaultMainApp theApp;
