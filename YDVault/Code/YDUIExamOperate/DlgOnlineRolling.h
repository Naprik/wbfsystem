#if !defined(AFX_DLGLINKROLLING_H__5184F070_0492_4CD6_8748_FE6DF173D4F1__INCLUDED_)
#define AFX_DLGLINKROLLING_H__5184F070_0492_4CD6_8748_FE6DF173D4F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgLinkrolling.h : header file
//

#include "resource.h"
#include "../UIBase\SortListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgOnlineRolling dialog
class CYDStuAppCom;

class CDlgOnlineRolling : public CDialog
{
// Construction
public:
	CDlgOnlineRolling(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOnlineRolling();
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
	//{{AFX_DATA(CDlgOnlineRolling)
	enum { IDD = IDD_YDUIEXAM_DLG_ONLINE_ROLLING };
	CSortListCtrl	m_linklist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOnlineRolling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOnlineRolling)
	afx_msg void OnLinkrolling();
	virtual BOOL OnInitDialog();
	afx_msg void OnSeach();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	std::list<CYDStuAppCom*> m_lstStuAppCom;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINKROLLING_H__5184F070_0492_4CD6_8748_FE6DF173D4F1__INCLUDED_)
