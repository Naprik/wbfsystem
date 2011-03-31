#if !defined(AFX_DLGSPEROLL_H__09185086_A09B_43FF_B4F1_C89BC79E125F__INCLUDED_)
#define AFX_DLGSPEROLL_H__09185086_A09B_43FF_B4F1_C89BC79E125F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
// Dlgsperoll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgsperoll dialog

class CDlgsperoll : public CDialog
{
// Construction
public:
	CDlgsperoll(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgsperoll)
	enum { IDD = IDD_DLG_SPEROLL };
	CSortListCtrl	m_list;
	CXPButton	m_speroll;
	CXPButton	m_spestart;
	CXPButton	m_cancel;
	CXPButton	m_ok;
	CString	m_stuid;
	CString	m_dev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgsperoll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgsperoll)
	afx_msg void OnSperoll();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSPEROLL_H__09185086_A09B_43FF_B4F1_C89BC79E125F__INCLUDED_)
