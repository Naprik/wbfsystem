#if !defined(AFX_DLGEXAMADDR_H__C548A9FC_C4C2_482D_AD18_53AC1B1B97F1__INCLUDED_)
#define AFX_DLGEXAMADDR_H__C548A9FC_C4C2_482D_AD18_53AC1B1B97F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgExamaddr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExamaddr dialog

class CDlgExamaddr : public CDialog
{
// Construction
public:
	CDlgExamaddr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExamaddr)
	enum { IDD = IDD_DIG_EXAMADDR };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CString	m_addr;
	CString	m_addid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExamaddr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExamaddr)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXAMADDR_H__C548A9FC_C4C2_482D_AD18_53AC1B1B97F1__INCLUDED_)
