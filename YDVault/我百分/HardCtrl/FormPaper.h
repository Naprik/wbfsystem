#if !defined(AFX_FORMPAPER_H__4F676F89_1592_4BD0_AC77_591B8AAAA8F0__INCLUDED_)
#define AFX_FORMPAPER_H__4F676F89_1592_4BD0_AC77_591B8AAAA8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBDepartment.h"
#include "DBSubject.h"
#include "DBPaper.h"
#include "DBClass.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// FormPaper.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormPaper dialog

class CFormPaper : public cdxCSizingDialog
{
// Construction
public:
	CFormPaper(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormPaper)

// Dialog Data
	//{{AFX_DATA(CFormPaper)
	enum { IDD = IDD_FORM_PAPER };
	CXPButton	m_delete;
	CXPButton	m_showall;
	CXPButton	m_query;
	CXPButton	m_add;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormPaper)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormPaper)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnShowall();
	afx_msg void OnQuerypaper();
	afx_msg void OnDeletepaper();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMPAPER_H__4F676F89_1592_4BD0_AC77_591B8AAAA8F0__INCLUDED_)
