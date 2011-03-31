#if !defined(AFX_DLGANSWER_H__265BC517_E8B1_47E0_9CEB_22E959E2822E__INCLUDED_)
#define AFX_DLGANSWER_H__265BC517_E8B1_47E0_9CEB_22E959E2822E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgAnswer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAnswer dialog

class CDlgAnswer : public CDialog
{
// Construction
public:
	

	CDlgAnswer(CWnd* pParent = NULL);   // standard constructor
	CString strSubject;
	int index;
// Dialog Data
	//{{AFX_DATA(CDlgAnswer)
	enum { IDD = IDD_DLG_ANSWER };
	CComboBox	m_subject;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	int		m_end;
	int		m_start;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnswer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnswer)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANSWER_H__265BC517_E8B1_47E0_9CEB_22E959E2822E__INCLUDED_)
