#if !defined(AFX_DLGLINKROLLING_H__5184F070_0492_4CD6_8748_FE6DF173D4F1__INCLUDED_)
#define AFX_DLGLINKROLLING_H__5184F070_0492_4CD6_8748_FE6DF173D4F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
#include "DBStuInfo.h"
#include "DBResult.h"
// DlgLinkrolling.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLinkrolling dialog

class CDlgLinkrolling : public CDialog
{
// Construction
public:
	CDlgLinkrolling(CWnd* pParent = NULL);   // standard constructor
	CStringArray	classarry;
	int				iStart;
	int             iEnd;
	int				iPaper;
	COleDateTime 	m_starttime;
	CString         sAddr;

	CString      sStuid;
	CString      sEndTime;
	CString      sStat;
//	COleDateTime 	m_endtime;
//	int				m_maxNO;
// Dialog Data
	//{{AFX_DATA(CDlgLinkrolling)
	enum { IDD = IDD_DIG_LINKROLLING };
	CSortListCtrl	m_stulist;
	CXPButton	m_search;
	CSortListCtrl	m_linklist;
	CXPButton	m_ok;
	CXPButton	m_linkrolling;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLinkrolling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLinkrolling)
	afx_msg void OnLinkrolling();
	virtual BOOL OnInitDialog();
	afx_msg void OnSeach();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINKROLLING_H__5184F070_0492_4CD6_8748_FE6DF173D4F1__INCLUDED_)
