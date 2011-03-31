#if !defined(AFX_DLGDPARTMENT_H__DF93AAFE_D6D3_4F2C_B318_E57AE1BD6523__INCLUDED_)
#define AFX_DLGDPARTMENT_H__DF93AAFE_D6D3_4F2C_B318_E57AE1BD6523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgDpartment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDpartment dialog

class CDlgDpartment : public CDialog
{
// Construction
public:
	CDlgDpartment(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDpartment)
	enum { IDD = IDD_DLG_DEPARTMENT };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CString	m_head;
	CString	m_tel;
	CString	m_depart;
	CString	m_departid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDpartment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDpartment)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
//	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDPARTMENT_H__DF93AAFE_D6D3_4F2C_B318_E57AE1BD6523__INCLUDED_)
