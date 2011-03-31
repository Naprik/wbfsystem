#if !defined(AFX_DLGCLASSRESULT_H__09469303_3A2E_425D_A333_6E866C65D809__INCLUDED_)
#define AFX_DLGCLASSRESULT_H__09469303_3A2E_425D_A333_6E866C65D809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgClassResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgClassResult dialog

class CDlgClassResult : public CDialog
{
// Construction
public:
	CDlgClassResult(CWnd* pParent = NULL);   // standard constructor

	CStringArray  subjectarry;
	CStringArray  classarry;
	CString       m_stusubject;
	CString       m_stuclass;

	CString strDepart;
	CString strSubject;
	CString strClass;
	CString strPaper;
// Dialog Data
	//{{AFX_DATA(CDlgClassResult)
	enum { IDD = IDD_DLG_CLASSRESULT };
	CComboBox	m_subject;
	CComboBox	m_paper;
	CComboBox	m_depart;
	CComboBox	m_class;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgClassResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgClassResult)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeDepart();
	afx_msg void OnSelchangeSubject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCLASSRESULT_H__09469303_3A2E_425D_A333_6E866C65D809__INCLUDED_)
