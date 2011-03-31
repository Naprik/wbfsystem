#if !defined(AFX_FORMEXAMROLLINGANSWER_H__C30AAF76_9024_4288_8DBB_AF82A41D1558__INCLUDED_)
#define AFX_FORMEXAMROLLINGANSWER_H__C30AAF76_9024_4288_8DBB_AF82A41D1558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
// FormExamRollingAnswer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamRollingAnswer dialog

class CFormExamRollingAnswer : public cdxCSizingDialog
{
// Construction
public:
	CFormExamRollingAnswer(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamRollingAnswer)
		
	void SetInfo();
	void SetKind(int kind);
	int  m_kind;		//进入item的编号，来自mainfram

// Dialog Data
	//{{AFX_DATA(CFormExamRollingAnswer)
	enum { IDD = IDD_FORM_ROLLING_INRAR_ANSWER };
	CXPButton	m_up;
	CXPButton	m_save;
	CSortListCtrl	m_list;
	CXPButton	m_fetch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamRollingAnswer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamRollingAnswer)
	virtual BOOL OnInitDialog();
	afx_msg void OnFetch1();
	afx_msg void OnSave();
	afx_msg void OnUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMROLLINGANSWER_H__C30AAF76_9024_4288_8DBB_AF82A41D1558__INCLUDED_)
