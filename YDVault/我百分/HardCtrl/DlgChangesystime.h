#if !defined(AFX_DLGCHANGESYSTIME_H__32E42E98_8A78_4EF0_B0E0_0992AFAA0884__INCLUDED_)
#define AFX_DLGCHANGESYSTIME_H__32E42E98_8A78_4EF0_B0E0_0992AFAA0884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChangesystime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChangesystime dialog

class CDlgChangesystime : public CDialog
{
// Construction
public:
	CDlgChangesystime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChangesystime)
	enum { IDD = IDD_CHANGESYSTIME };
	COleDateTime	m_date;
	COleDateTime	m_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChangesystime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChangesystime)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANGESYSTIME_H__32E42E98_8A78_4EF0_B0E0_0992AFAA0884__INCLUDED_)
