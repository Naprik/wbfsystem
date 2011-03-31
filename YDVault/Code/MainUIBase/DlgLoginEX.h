//{{AFX_INCLUDES()
#include "../UIBase/shockwaveflash.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGLOGINEX_H__5C7FAC05_4D90_47DA_9BDF_53B335D194FB__INCLUDED_)
#define AFX_DLGLOGINEX_H__5C7FAC05_4D90_47DA_9BDF_53B335D194FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../UIBase/cdxCSizingDialog.h"
#include "../UIBase/ColorStatic.h"
#include "Resource.h"

// DlgLoginEX.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLoginEX dialog

class CDatabaseEx;

class AFX_EXT_CLASS CDlgLoginEX : public CBCGPDialog
{
// Construction
public:
	CDlgLoginEX(CWnd* pParent = NULL);   // standard constructor
	CDC m_backDC;
	CBitmap        m_bmpBackground;
// Dialog Data
	//{{AFX_DATA(CDlgLoginEX)
	enum { IDD = IDD_MAINUIBASE_DLG_LOGIN };
	
	CEdit	m_passctl;
	CShockwaveFlash	m_flash;
	CString	m_pass;
	CString	m_user;
	CDatabaseEx* m_pDatabase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLoginEX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLoginEX)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	afx_msg void OnStnClickedStaticCopyright();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGINEX_H__5C7FAC05_4D90_47DA_9BDF_53B335D194FB__INCLUDED_)
