#if !defined(AFX_DLGTEACHERDEV_H__0910CA6B_8836_4F44_BC19_20428F960FC8__INCLUDED_)
#define AFX_DLGTEACHERDEV_H__0910CA6B_8836_4F44_BC19_20428F960FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgTeacherdev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherdev dialog

class CDlgTeacherdev : public CDialog
{
// Construction
public:
	CDlgTeacherdev(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTeacherdev)
	enum { IDD = IDD_DIG_TEACHERDEV };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CString	m_devmac;
	CString	m_devid;
	CString	m_teachername;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTeacherdev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTeacherdev)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEACHERDEV_H__0910CA6B_8836_4F44_BC19_20428F960FC8__INCLUDED_)
