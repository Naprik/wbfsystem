#if !defined(AFX_DLGLINKROLL_H__616BD0B2_972A_4546_B816_8C04FC3C1770__INCLUDED_)
#define AFX_DLGLINKROLL_H__616BD0B2_972A_4546_B816_8C04FC3C1770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgLinkroll.h : header file
//

#include "Resource.h"
#include "../UIBase\SortListCtrl.h"

class CYDStuAppCom;

/////////////////////////////////////////////////////////////////////////////
// CDlgOnlineBeginExam dialog

class CDlgOnlineBeginExam : public CDialog
{
// Construction
public:
	CDlgOnlineBeginExam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOnlineBeginExam();

	CStringArray  classarry;
	COleDateTime 	m_starttime;
	COleDateTime 	m_endtime;
	int				m_maxNO;

	CString      sStuid;
	CString      sStartTime;
	CString      sStat;

// Dialog Data
	//{{AFX_DATA(CDlgOnlineBeginExam)
	enum { IDD = IDD_YDUIEXAM_DLG_ONLINE_BEGINEXAM };
	CSortListCtrl	m_examlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOnlineBeginExam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOnlineBeginExam)
	afx_msg void OnOnLineBeginExam();
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	std::list<CYDStuAppCom*> m_lstStuAppCom;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINKROLL_H__616BD0B2_972A_4546_B816_8C04FC3C1770__INCLUDED_)
