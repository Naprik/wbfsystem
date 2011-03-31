#if !defined(AFX_DLGSUB_H__E2F0AACB_A825_4CF8_B0C0_BA5EB971A941__INCLUDED_)
#define AFX_DLGSUB_H__E2F0AACB_A825_4CF8_B0C0_BA5EB971A941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSub.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftManage dialog
#include "cdxCSizingDialog.h"
class CLeftManage : public cdxCSizingDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CLeftManage)
	CLeftManage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLeftManage)
	enum { IDD = IDD_LEFT_MANAGE };
	CButton	m_group;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftManage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeftManage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnOutbarNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CXTOutBarCtrl  m_wndOutlookBar;	
	CImageList	   m_ImageLarge;
	CImageList	   m_ImageSmall;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUB_H__E2F0AACB_A825_4CF8_B0C0_BA5EB971A941__INCLUDED_)
