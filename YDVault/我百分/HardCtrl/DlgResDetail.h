#if !defined(AFX_DLGRESDETAIL_H__A6BA5197_0C28_459A_B554_1B22D0EBE9F3__INCLUDED_)
#define AFX_DLGRESDETAIL_H__A6BA5197_0C28_459A_B554_1B22D0EBE9F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
#include "MyList.h"
#include "DBResult.h"
#include "DBStuInfo.h"
#include "DBPaper.h"
#include "DBStdanswer.h"
// DlgResDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgResDetail dialog

class CDlgResDetail : public CDialog
{
// Construction
public:
	CDlgResDetail(CWnd* pParent = NULL);   // standard constructor
	int iPaperID;
	CString strStuid;
	CString strStuname;
	CString strSubject;
	CString strPaper;
	CString strRight;
	CString strWorng;
	CString strSocrt;


// Dialog Data
	//{{AFX_DATA(CDlgResDetail)
	enum { IDD = IDD_DIG_RES_DETAIL };
	CMyList	m_reslist;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgResDetail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgResDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkReslist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESDETAIL_H__A6BA5197_0C28_459A_B554_1B22D0EBE9F3__INCLUDED_)
