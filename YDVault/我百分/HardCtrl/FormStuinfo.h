#if !defined(AFX_FORMSTUINFO_H__54AEE34B_90AA_4239_B139_F7DB70D9D56B__INCLUDED_)
#define AFX_FORMSTUINFO_H__54AEE34B_90AA_4239_B139_F7DB70D9D56B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBStuInfo.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// FormStuinfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormStuinfo dialog

class CFormStuinfo : public cdxCSizingDialog
{
// Construction
public:
	CFormStuinfo(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormStuinfo)
// Dialog Data
	//{{AFX_DATA(CFormStuinfo)
	enum { IDD = IDD_FORM_STUINFO };
	CXPButton	m_query;
	CSortListCtrl	m_stulist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormStuinfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFormStuinfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuery();
	afx_msg void OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMSTUINFO_H__54AEE34B_90AA_4239_B139_F7DB70D9D56B__INCLUDED_)
