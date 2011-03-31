#if !defined(AFX_DLGPHOTO_H__6E7AC1B9_09CE_40B4_B375_173A25DAC94A__INCLUDED_)
#define AFX_DLGPHOTO_H__6E7AC1B9_09CE_40B4_B375_173A25DAC94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
// DlgPhoto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPhoto dialog

class CDlgPhoto : public CDialog
{
// Construction
public:
	CDlgPhoto(CWnd* pParent = NULL);   // standard constructor
	CString   m_path;
	CString   m_id;
	CString	  m_name;
	CString	  m_gender;
	CString	  m_class;
	CString	  m_depart;
	CString	  m_tel;
	CString	  m_email;
// Dialog Data
	//{{AFX_DATA(CDlgPhoto)
	enum { IDD = IDD_DIG_PHOTO };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CButton		m_photo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPhoto)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPhoto)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPHOTO_H__6E7AC1B9_09CE_40B4_B375_173A25DAC94A__INCLUDED_)
