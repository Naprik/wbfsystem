#if !defined(AFX_FORMEXAMIMPORT_H__5367933A_5028_47D6_8C31_8CF9CBABD619__INCLUDED_)
#define AFX_FORMEXAMIMPORT_H__5367933A_5028_47D6_8C31_8CF9CBABD619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "ExamDetail.h"
// FormExamImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamImport dialog

class CFormExamImport : public cdxCSizingDialog
{
// Construction
public:
	CFormExamImport(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamImport)

// Dialog Data
	//{{AFX_DATA(CFormExamImport)
	enum { IDD = IDD_FORM_EXAM_IMPORT };
	CXPButton	m_save;
	CXPButton	m_import;
	CSortListCtrl	m_stulist;
	CSortListCtrl	m_paperlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamImport)
	virtual BOOL OnInitDialog();
	afx_msg void OnImport();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CExamDetail  m_detail;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMIMPORT_H__5367933A_5028_47D6_8C31_8CF9CBABD619__INCLUDED_)
