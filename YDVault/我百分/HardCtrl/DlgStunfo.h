#if !defined(AFX_DLGSTUNFO_H__91C809BA_6CF8_439C_A664_A3A521B2F65A__INCLUDED_)
#define AFX_DLGSTUNFO_H__91C809BA_6CF8_439C_A664_A3A521B2F65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "DBDepartment.h"
// DlgStunfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStunfo dialog

class CDlgStunfo : public CDialog
{
// Construction
public:
	CDlgStunfo(CWnd* pParent = NULL);   // standard constructor

	CString		strPath;
	CString		strSex;
	CString     strDepart;
	CString     strClass;
// Dialog Data
	//{{AFX_DATA(CDlgStunfo)
	enum { IDD = IDD_DLG_STUINFO };
	CComboBox	m_department;
	CComboBox	m_class;
	CXPButton	m_button;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CComboBox	m_gender;
	CString		m_email;
	CString		m_name;
	CString		m_num;
	CString		m_tel;
	CString	m_blueteeth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStunfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDlgStunfo)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeDepartment();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTUNFO_H__91C809BA_6CF8_439C_A664_A3A521B2F65A__INCLUDED_)
