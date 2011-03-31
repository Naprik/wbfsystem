// HardCtrl.h : main header file for the HARDCTRL application
//

#if !defined(AFX_HARDCTRL_H__7B0611FB_A10D_4F2D_BCE0_4F46277B52A9__INCLUDED_)
#define AFX_HARDCTRL_H__7B0611FB_A10D_4F2D_BCE0_4F46277B52A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       
class CHardCtrlApp : public CWinApp
{
public:
	CHardCtrlApp();
	//{{AFX_VIRTUAL(CHardCtrlApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CHardCtrlApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetXpStype(BOOL flag);
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_HARDCTRL_H__7B0611FB_A10D_4F2D_BCE0_4F46277B52A9__INCLUDED_)
