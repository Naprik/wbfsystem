#if !defined(AFX_FORMEXAMINRAROFF_H__6659CB06_5A3F_4B62_9080_E8ED4881C8F0__INCLUDED_)
#define AFX_FORMEXAMINRAROFF_H__6659CB06_5A3F_4B62_9080_E8ED4881C8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
// FormExamInrarOff.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamInrarOff dialog

class CFormExamInrarOff : public cdxCSizingDialog
{
// Construction
public:
	CFormExamInrarOff(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamInrarOff)
	void SetInfo();
	void SetKind(int kind);
	int m_kind;		//进入item的编号，来自mainfram
	void setdetail();
// Dialog Data
	//{{AFX_DATA(CFormExamInrarOff)
	enum { IDD = IDD_FORM_EXAM_INRARA_OFF };
	CXPButton	m_sendinfo;
	CXPButton	m_import;
	CXPButton	m_edit;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamInrarOff)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamInrarOff)
	virtual BOOL OnInitDialog();
	afx_msg void OnImport();
	afx_msg void OnEdit();
	afx_msg void OnSendinfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMINRAROFF_H__6659CB06_5A3F_4B62_9080_E8ED4881C8F0__INCLUDED_)
