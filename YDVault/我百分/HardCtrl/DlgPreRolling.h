#if !defined(AFX_DLGPREROLLING_H__FC654BFF_32F8_40F9_B8A1_C3EBECB778C2__INCLUDED_)
#define AFX_DLGPREROLLING_H__FC654BFF_32F8_40F9_B8A1_C3EBECB778C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgPreRolling.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPreRolling dialog

class CDlgPreRolling : public CDialog
{
// Construction
public:
	CDlgPreRolling(CWnd* pParent = NULL);   // standard constructor
	CString   m_path;
	CString   m_id;
	CString	  m_name;
	CString	  m_gender;
	CString	  m_depart;
	

	CString   m_suject;
	CString   m_addr;
	CString   m_start;
	CString   m_end;

// Dialog Data
	//{{AFX_DATA(CDlgPreRolling)
	enum { IDD = IDD_DLG_PRE_ROLLING };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CButton	m_photo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPreRolling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPreRolling)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPREROLLING_H__FC654BFF_32F8_40F9_B8A1_C3EBECB778C2__INCLUDED_)
