#if !defined(AFX_DLGEXAMSTAT_H__8B42CE1E_B3D2_49D8_8B81_1C992BC0DF16__INCLUDED_)
#define AFX_DLGEXAMSTAT_H__8B42CE1E_B3D2_49D8_8B81_1C992BC0DF16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgExamstat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExamstat dialog

class CDlgExamstat : public CDialog
{
// Construction
public:
	CDlgExamstat(CWnd* pParent = NULL);   // standard constructor

	CString    m_original;
	CString    m_reality;
	CString    m_subject;

// Dialog Data
	//{{AFX_DATA(CDlgExamstat)
	enum { IDD = IDD_DIG_EXAM_STAT };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExamstat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExamstat)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXAMSTAT_H__8B42CE1E_B3D2_49D8_8B81_1C992BC0DF16__INCLUDED_)
