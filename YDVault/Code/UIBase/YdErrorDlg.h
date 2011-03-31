#if !defined(AFX_YDERRORDLG_H__7E9B9ED4_AE73_43C4_BE2A_B8E337C9E833__INCLUDED_)
#define AFX_YDERRORDLG_H__7E9B9ED4_AE73_43C4_BE2A_B8E337C9E833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YdErrorDlg.h : header file
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CYdErrorDlg dialog

class BASE_DLL_API CYdErrorDlg : public CDialog
{
// Construction
public:
	CYdErrorDlg(const CString& strMsg,HRESULT hr,
		UINT nIcon=MB_OK|MB_ICONSTOP,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYdErrorDlg)
	enum { IDD = IDD_UIBASE_ERROR };
	CStatic	m_ctrlIcon;
	//}}AFX_DATA
	
	CString		m_strMsg;
	HRESULT		m_hr;
	UINT		m_nIcon;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYdErrorDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYdErrorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YDERRORDLG_H__7E9B9ED4_AE73_43C4_BE2A_B8E337C9E833__INCLUDED_)
