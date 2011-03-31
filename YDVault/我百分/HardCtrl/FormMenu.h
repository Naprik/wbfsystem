#if !defined(AFX_FORMMENU_H__6425848B_C9CC_49D2_9AC3_410FC090DE7F__INCLUDED_)
#define AFX_FORMMENU_H__6425848B_C9CC_49D2_9AC3_410FC090DE7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "ControlButton.h"
// FormMenu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormMenu dialog

class CFormMenu : public cdxCSizingDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CFormMenu)
	CFormMenu(CWnd* pParent = NULL);   // standard constructor
	CBitmap        m_bmpBackground;
// Dialog Data
	//{{AFX_DATA(CFormMenu)
	enum { IDD = IDD_FORM_MENU };
	CControlButton	m_query;
	CControlButton	m_exit;
	CControlButton	m_exam;
	CControlButton	m_date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormMenu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormMenu)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnExam();
	afx_msg void OnDate();
	afx_msg void OnQuery();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMMENU_H__6425848B_C9CC_49D2_9AC3_410FC090DE7F__INCLUDED_)
