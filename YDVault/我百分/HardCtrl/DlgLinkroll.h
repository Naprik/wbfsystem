#if !defined(AFX_DLGLINKROLL_H__616BD0B2_972A_4546_B816_8C04FC3C1770__INCLUDED_)
#define AFX_DLGLINKROLL_H__616BD0B2_972A_4546_B816_8C04FC3C1770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
#include "DBStuInfo.h"
// DlgLinkroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLinkroll dialog

class CDlgLinkroll : public CDialog
{
// Construction
public:
	CDlgLinkroll(CWnd* pParent = NULL);   // standard constructor

	CStringArray  classarry;
	COleDateTime 	m_starttime;
	COleDateTime 	m_endtime;
	int				m_maxNO;

	CString      sStuid;
	CString      sStartTime;
	CString      sStat;

// Dialog Data
	//{{AFX_DATA(CDlgLinkroll)
	enum { IDD = IDD_DLG_LINKROLL };
	CXPButton	m_search;
	CSortListCtrl	m_examlist;
	CSortListCtrl	m_stulist;
	CXPButton	m_ok;
	CXPButton	m_linkroll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLinkroll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLinkroll)
	afx_msg void OnLinkroll();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearch();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINKROLL_H__616BD0B2_972A_4546_B816_8C04FC3C1770__INCLUDED_)
