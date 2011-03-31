#if !defined(AFX_DLGEXMOPTION_H__17C1F664_185D_48E0_BE98_9DE7EA580D4D__INCLUDED_)
#define AFX_DLGEXMOPTION_H__17C1F664_185D_48E0_BE98_9DE7EA580D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
// DlgExmOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExmOption dialog

class CDlgExmOption : public CDialog
{
// Construction
public:
	CDlgExmOption(CWnd* pParent = NULL);   // standard constructor
	CStringArray  subjectarry;
	CStringArray  addrarry;
	CStringArray  classarry;
// Dialog Data
	//{{AFX_DATA(CDlgExmOption)
	enum { IDD = IDD_DIG_EXMOPTION };
	CSortListCtrl	m_classlist;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CComboBox	m_examclass;
	CComboBox	m_address;
	CComboBox	m_subject;
	COleDateTime	m_start;
	COleDateTime	m_end;
	CString		m_supervisor;
	CString	    m_stusubject;
	CString     m_examaddr;
	int		m_account;
	int		m_maxno;
	int		m_startno;
	int		m_endno;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExmOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExmOption)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXMOPTION_H__17C1F664_185D_48E0_BE98_9DE7EA580D4D__INCLUDED_)
