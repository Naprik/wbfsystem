#if !defined(AFX_SETPASS_H__4FFF4FA0_88B7_41A9_BBC9_BE17276546E3__INCLUDED_)
#define AFX_SETPASS_H__4FFF4FA0_88B7_41A9_BBC9_BE17276546E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setpass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetpass dialog

class CSetpass : public CDialog
{
// Construction
public:
	CSetpass(CWnd* pParent = NULL);   // standard constructor
	CString name;
// Dialog Data
	//{{AFX_DATA(CSetpass)
	enum { IDD = IDD_SETPASS };
	CString	m_setpass1;
	CString	m_setpass2;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetpass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetpass)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPASS_H__4FFF4FA0_88B7_41A9_BBC9_BE17276546E3__INCLUDED_)
