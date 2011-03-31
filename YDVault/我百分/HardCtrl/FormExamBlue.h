#if !defined(AFX_FORMEXAMBLUE_H__605EF460_C37B_4A5F_A665_2BF54B82FA0B__INCLUDED_)
#define AFX_FORMEXAMBLUE_H__605EF460_C37B_4A5F_A665_2BF54B82FA0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "ControlButton.h"
// FormExamBlue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamBlue dialog

class CFormExamBlue : public cdxCSizingDialog
{
// Construction
public:
	CFormExamBlue(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamBlue)

	void SetInfo();
	void SetKind(int kind);
	int m_kind;		//进入item的编号，来自mainfram

	int m_account;

	void setdetail();

// Dialog Data
	//{{AFX_DATA(CFormExamBlue)
	enum { IDD = IDD_FORM_EXAM_BLUE };
	CXPButton	m_up;
	CXPButton	m_importclass;
	CXPButton	m_import;
	CXPButton	m_export;
	CXPButton	m_edit;
	CXPButton	m_search;
	CXPButton	m_rolling;
	CXPButton	m_roll;
	CSortListCtrl	m_stulist;
	CString	m_systime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamBlue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamBlue)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUp();
	afx_msg void OnSearch();
	afx_msg void OnRoll();
	afx_msg void OnRolling();
	afx_msg void OnExport();
	afx_msg void OnImport();
	afx_msg void OnEdit();
	afx_msg void OnImportclass();
	afx_msg void OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_isroll;
	bool m_isrolling;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMBLUE_H__605EF460_C37B_4A5F_A665_2BF54B82FA0B__INCLUDED_)
