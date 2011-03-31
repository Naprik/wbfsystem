#if !defined(AFX_FORMLLINK_H__043AC597_6FD2_463E_8361_38517D791F04__INCLUDED_)
#define AFX_FORMLLINK_H__043AC597_6FD2_463E_8361_38517D791F04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ControlButton.h"
// FormlLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormlLink dialog

class CFormlLink : public cdxCSizingDialog
{
// Construction
public:
	CFormlLink(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormlLink)
// Dialog Data
	//{{AFX_DATA(CFormlLink)
	enum { IDD = IDD_FORM_LINK };
	CControlButton	m_test;
	CControlButton	m_choose;
	CSortListCtrl	m_teacherlist;
	CButton	m_tt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormlLink)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL bIscom1, bIscom2, bIscom3;
	// Generated message map functions
	//{{AFX_MSG(CFormlLink)
	virtual BOOL OnInitDialog();
	afx_msg void OnTest();
	afx_msg void OnChoose();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	//CShadeButtonST	m_testButtons;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMLLINK_H__043AC597_6FD2_463E_8361_38517D791F04__INCLUDED_)
