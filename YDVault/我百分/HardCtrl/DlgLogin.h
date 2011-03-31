#if !defined(AFX_DLGLOGIN_H__6C00DED1_7F32_40ED_8E3D_6E423863EBAB__INCLUDED_)
#define AFX_DLGLOGIN_H__6C00DED1_7F32_40ED_8E3D_6E423863EBAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "XPButton.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// DlgLogin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin dialog

class CDlgLogin : public CDialog
{
// Construction
public:

	CDlgLogin(CWnd* pParent = NULL);   // standard constructor
	CDC m_backDC;
	CBitmap        m_bmpBackground;
// Dialog Data
	//{{AFX_DATA(CDlgLogin)
	enum { IDD = IDD_DLG_LOG_IN };
	CControlButton	m_ok;
	CControlButton	m_cancel;
	CString	m_pass;
	CString	m_user;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetExePath();

//	CShadeButtonST	m_logButtons;
//	CShadeButtonST	m_outButtons;
	// Generated message map functions
	//{{AFX_MSG(CDlgLogin)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CColorStatic m_Static1;
	CColorStatic m_Static2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGIN_H__6C00DED1_7F32_40ED_8E3D_6E423863EBAB__INCLUDED_)
