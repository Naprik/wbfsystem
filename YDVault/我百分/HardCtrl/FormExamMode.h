#if !defined(AFX_FORMEXAMMODE_H__BE85AC07_3BE1_4BBC_8B53_E43E31D130D9__INCLUDED_)
#define AFX_FORMEXAMMODE_H__BE85AC07_3BE1_4BBC_8B53_E43E31D130D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "ControlButton.h"
// FormExamMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamMode dialog

class CFormExamMode : public cdxCSizingDialog
{
// Construction
public:
	CFormExamMode(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamMode)

	void SetKind(int kind);
	int m_kind;        //进入item的编号，来自mainfram；
// Dialog Data
	//{{AFX_DATA(CFormExamMode)
	enum { IDD = IDD_FORM_EXAM_MODE };
	CControlButton	m_search;
	CControlButton	m_choose;
	CSortListCtrl	m_devlist;
	CControlButton	m_next;
	CButton	m_blue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamMode)
	virtual BOOL OnInitDialog();
	afx_msg void OnNext();
	afx_msg void OnSearch();
	afx_msg void OnChoose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMMODE_H__BE85AC07_3BE1_4BBC_8B53_E43E31D130D9__INCLUDED_)
