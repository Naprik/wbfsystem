#if !defined(AFX_DLGWEB_H__C8B4A108_2C34_4F0A_BCFD_92C1D042312A__INCLUDED_)
#define AFX_DLGWEB_H__C8B4A108_2C34_4F0A_BCFD_92C1D042312A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWeb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftReport dialog
#include "cdxCSizingDialog.h"
class CLeftReport : public cdxCSizingDialog
{
// Construction
public:
    DECLARE_DYNCREATE(CLeftReport)
	CLeftReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLeftReport)
	enum { IDD = IDD_LEFT_REPORT };
	CButton	m_group;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLeftReport)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
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

#endif // !defined(AFX_DLGWEB_H__C8B4A108_2C34_4F0A_BCFD_92C1D042312A__INCLUDED_)
