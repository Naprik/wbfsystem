#if !defined(AFX_DLGPAPERANALY_H__BB78AF26_4297_4BCC_BC71_A1102C2D1BD3__INCLUDED_)
#define AFX_DLGPAPERANALY_H__BB78AF26_4297_4BCC_BC71_A1102C2D1BD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DBDepartment.h"
#include "DBSubject.h"
#include "DBPaper.h"
// DlgPaperAnaly.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperAnaly dialog

class CDlgPaperAnaly : public CDialog
{
// Construction
public:
	CDlgPaperAnaly(CWnd* pParent = NULL);   // standard constructor

	CString strSubject;
	CString strPaper;
	CStringArray  arryClass;
// Dialog Data
	//{{AFX_DATA(CDlgPaperAnaly)
	enum { IDD = IDD_DLG_PAPER_ANALY };
	CComboBox	m_subject;
	CComboBox	m_paper;
	CSortListCtrl	m_classlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPaperAnaly)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPaperAnaly)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSubject();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAPERANALY_H__BB78AF26_4297_4BCC_BC71_A1102C2D1BD3__INCLUDED_)
