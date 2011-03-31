#if !defined(AFX_DLGFM_H__BE65E8D7_A7CF_4FB4_B641_335E7E5A1263__INCLUDED_)
#define AFX_DLGFM_H__BE65E8D7_A7CF_4FB4_B641_335E7E5A1263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../UIBase/XPButton.h"
// DlgFM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFM dialog
#include "Resource.h"

class CDlgFM : public CDialog
{
// Construction
public:
	CDlgFM(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFM)
	enum { IDD = IDD_YDUIEXAM_DLG_FM };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CString		m_channel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFM)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFM_H__BE65E8D7_A7CF_4FB4_B641_335E7E5A1263__INCLUDED_)
