#if !defined(AFX_FORMSUBJECT_H__8ED29C1D_A1A4_498F_AB3A_55E95DC384DC__INCLUDED_)
#define AFX_FORMSUBJECT_H__8ED29C1D_A1A4_498F_AB3A_55E95DC384DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBSubject.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// FormSubject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormSubject dialog

class CFormSubject : public cdxCSizingDialog
{
// Construction
public:
	CFormSubject(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormSubject)
// Dialog Data
	//{{AFX_DATA(CFormSubject)
	enum { IDD = IDD_FORM_SUBJECT };
	CXPButton	m_update;
	CXPButton	m_showall;
	CXPButton	m_delete;
	CXPButton	m_add;
	CSortListCtrl	m_subjectlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormSubject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void		 showdata();
	// Generated message map functions
	//{{AFX_MSG(CFormSubject)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnUpdate();
	afx_msg void OnDelete();
	afx_msg void OnShowall();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMSUBJECT_H__8ED29C1D_A1A4_498F_AB3A_55E95DC384DC__INCLUDED_)
