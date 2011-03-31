#if !defined(AFX_FORMEXAMINFOBACK_H__7C3BA7D2_15E6_4F46_BFD2_31EE763CF8A3__INCLUDED_)
#define AFX_FORMEXAMINFOBACK_H__7C3BA7D2_15E6_4F46_BFD2_31EE763CF8A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
// FormExamInfoBack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfoBack dialog

class CFormExamInfoBack : public cdxCSizingDialog
{
// Construction
public:
	CFormExamInfoBack(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamInfoBack)

	void SetInfo();
	void SetKind(int kind);
	int m_kind;		//进入item的编号，来自mainfram

// Dialog Data
	//{{AFX_DATA(CFormExamInfoBack)
	enum { IDD = IDD_FORM_EXMA_INFO_BACK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamInfoBack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamInfoBack)
	virtual BOOL OnInitDialog();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMINFOBACK_H__7C3BA7D2_15E6_4F46_BFD2_31EE763CF8A3__INCLUDED_)
