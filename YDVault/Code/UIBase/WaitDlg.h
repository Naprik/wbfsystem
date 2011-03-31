#if !defined(AFX_WAITDLG_H__A0020215_EBAA_446F_8924_A9DAC069DC2B__INCLUDED_)
#define AFX_WAITDLG_H__A0020215_EBAA_446F_8924_A9DAC069DC2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog
class CPdemWait;

class CWaitDlg : public CDialog
{
// Construction
public:
	CWaitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaitDlg)
	enum { IDD = IDD_UIBASE_WAIT };
	CProgressCtrl	m_prgctrlWait;
	CString	m_strWaitMsg;
	//}}AFX_DATA
	CPdemWait		*m_pWait;
	UINT			 m_nRange;
	short            m_iDelaySecond;
	BOOL			 m_bShowCancel;

	int              m_nCount;
	UINT             m_nIDTimer;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITDLG_H__A0020215_EBAA_446F_8924_A9DAC069DC2B__INCLUDED_)
