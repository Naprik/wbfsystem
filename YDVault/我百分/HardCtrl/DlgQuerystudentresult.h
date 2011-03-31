#if !defined(AFX_DLGQUERYSTUDENTRESULT_H__FF1C864E_C8B3_4E10_8213_4DD9D11D9549__INCLUDED_)
#define AFX_DLGQUERYSTUDENTRESULT_H__FF1C864E_C8B3_4E10_8213_4DD9D11D9549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "DBDepartment.h"
#include "DBStuInfo.h"
#include "DBSubject.h"
#include "DBPaper.h"
// DlgQuerystudentresult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQuerystudentresult dialog

class CDlgQuerystudentresult : public CDialog
{
// Construction
public:
	CDlgQuerystudentresult(CWnd* pParent = NULL);   // standard constructor
	CString strSubject;
	CString	strPaper;
	CString strName;
	CString strID;
	BOOL	 bIstime;
// Dialog Data
	//{{AFX_DATA(CDlgQuerystudentresult)
	enum { IDD = IDD_DIG_STURESULT_QUERY };
	CEdit	m_editname;
	CEdit	m_editid;
	CDateTimeCtrl	m_datectl;
	CComboBox	m_paper;
	CComboBox	m_sub;
	CComboBox	m_name;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	COleDateTime	m_datetime;
	CString	m_stuid;
	CString	m_stuname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgQuerystudentresult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgQuerystudentresult)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSelchangeSub();
	afx_msg void OnAll();
	afx_msg void OnSubjectradio();
	afx_msg void OnTime();
	afx_msg void OnStudnum();
	afx_msg void OnName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQUERYSTUDENTRESULT_H__FF1C864E_C8B3_4E10_8213_4DD9D11D9549__INCLUDED_)
