#if !defined(AFX_FORMIMPORT_H__AD7A3C20_9542_40F0_BA7D_AA6C4134FD34__INCLUDED_)
#define AFX_FORMIMPORT_H__AD7A3C20_9542_40F0_BA7D_AA6C4134FD34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
// FormImport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormImport dialog

class CFormImport : public cdxCSizingDialog
{
// Construction
public:
	CFormImport(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormImport)

	void SetInfo();
	void SetKind(int kind);
	int  m_kind;
// Dialog Data
	//{{AFX_DATA(CFormImport)
	enum { IDD = IDD_FORM_IMPORT };
	CXPButton	m_export;
	CXPButton	m_save;
	CSortListCtrl	m_paperlist;
	CSortListCtrl	m_answerlist;
	CXPButton	m_import;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormImport)
	afx_msg void OnImport();
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString GetExePath();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMIMPORT_H__AD7A3C20_9542_40F0_BA7D_AA6C4134FD34__INCLUDED_)
