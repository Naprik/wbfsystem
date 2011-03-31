#if !defined(AFX_DLGSINGLEROLL_H__94F5B5B5_162E_496A_959E_FA5FEBB34889__INCLUDED_)
#define AFX_DLGSINGLEROLL_H__94F5B5B5_162E_496A_959E_FA5FEBB34889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgSingleroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSingleroll dialog

class CDlgSingleroll : public CDialog
{
// Construction
public:
	CDlgSingleroll(CWnd* pParent = NULL);   // standard constructor
	
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
	//{{AFX_DATA(CDlgSingleroll)
	enum { IDD = IDD_DIG_SINGLEROLL };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CButton	m_photo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSingleroll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSingleroll)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSINGLEROLL_H__94F5B5B5_162E_496A_959E_FA5FEBB34889__INCLUDED_)
