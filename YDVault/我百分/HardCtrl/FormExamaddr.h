#if !defined(AFX_FORMEXAMADDR_H__6081E998_50B6_4E4F_B630_BE6B89B5C647__INCLUDED_)
#define AFX_FORMEXAMADDR_H__6081E998_50B6_4E4F_B630_BE6B89B5C647__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBExamaddr.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"

class CFormExamaddr : public cdxCSizingDialog
{
// Construction
public:
	CFormExamaddr(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamaddr)
// Dialog Data
	//{{AFX_DATA(CFormExamaddr)
	enum { IDD = IDD_FORM_EXAMADDR };
	CXPButton	m_update;
	CXPButton	m_showall;
	CXPButton	m_delete;
	CXPButton	m_add;
	CSortListCtrl	m_addrlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamaddr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void		 showdata();
	// Generated message map functions
	//{{AFX_MSG(CFormExamaddr)
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

#endif // !defined(AFX_FORMEXAMADDR_H__6081E998_50B6_4E4F_B630_BE6B89B5C647__INCLUDED_)
