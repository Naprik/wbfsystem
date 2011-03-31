#if !defined(AFX_DLGCTRL_H__26D4A5F9_3C03_4F9C_BC0D_AB08A15C87D2__INCLUDED_)
#define AFX_DLGCTRL_H__26D4A5F9_3C03_4F9C_BC0D_AB08A15C87D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftAudit dialog
#include "cdxCSizingDialog.h"
class CLeftAudit : public cdxCSizingDialog
{
// Construction
public:
	CLeftAudit(CWnd* pParent = NULL);   // standard constructor
    DECLARE_DYNCREATE(CLeftAudit)
// Dialog Data
	//{{AFX_DATA(CLeftAudit)
	enum { IDD = IDD_LEFT_AUDIT };
	CButton	m_group;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftAudit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CLeftAudit)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnOutbarNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CXTOutBarCtrl  m_wndOutlookBar;	
	CImageList	   m_ImageLarge;
	CImageList	   m_ImageSmall;
	CBitmap        m_bmpBackground;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCTRL_H__26D4A5F9_3C03_4F9C_BC0D_AB08A15C87D2__INCLUDED_)
