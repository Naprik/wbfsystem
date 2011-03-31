#if !defined(AFX_DLGCLASSQUERY_H__63221B2D_7DAA_47F7_9836_460D0DFA6689__INCLUDED_)
#define AFX_DLGCLASSQUERY_H__63221B2D_7DAA_47F7_9836_460D0DFA6689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "DBDepartment.h"
// DlgClassQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgClassQuery dialog

class CDlgClassQuery : public CDialog
{
// Construction
public:
	CDlgClassQuery(CWnd* pParent = NULL);   // standard constructor
	CStringArray  classarry;

// Dialog Data
	//{{AFX_DATA(CDlgClassQuery)
	enum { IDD = IDD_DLG_CLASS_QUERY };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CSortListCtrl	m_classlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgClassQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgClassQuery)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCLASSQUERY_H__63221B2D_7DAA_47F7_9836_460D0DFA6689__INCLUDED_)
