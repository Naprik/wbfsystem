#if !defined(AFX_FORMEXAMDETAIL_H__CAEDB736_78B8_4DF9_8E80_E092163FCFA1__INCLUDED_)
#define AFX_FORMEXAMDETAIL_H__CAEDB736_78B8_4DF9_8E80_E092163FCFA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
// FormExamDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamDetail dialog

class CFormExamDetail : public cdxCSizingDialog
{
// Construction
public:
	CFormExamDetail(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamDetail)
	void SetInfo();
	void SetKind(int kind);
	int  m_kind;		//进入item的编号，来自mainfram

// Dialog Data
	//{{AFX_DATA(CFormExamDetail)
	enum { IDD = IDD_FORM_EXAM_DETAIL };
	CXPButton	m_search;
	CXPButton	m_next;
	CXPButton	m_choose;
	CSortListCtrl	m_devlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamDetail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnChoose();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMDETAIL_H__CAEDB736_78B8_4DF9_8E80_E092163FCFA1__INCLUDED_)
