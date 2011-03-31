#if !defined(AFX_FORMPCOFFROLL_H__8EA64316_CA73_442E_A94B_B85FE79B181A__INCLUDED_)
#define AFX_FORMPCOFFROLL_H__8EA64316_CA73_442E_A94B_B85FE79B181A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBStuInfo.h"
#include "DBPaper.h"
#include "DBTeacherDev.h"

// FormPCoffRoll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormPCoffRoll dialog

class CFormPCoffRoll : public cdxCSizingDialog
{
// Construction
public:
	CFormPCoffRoll(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormPCoffRoll)
// Dialog Data
	//{{AFX_DATA(CFormPCoffRoll)
	enum { IDD = IDD_FORM_PCOFF_ROLL };
	CXPButton	m_next;
	CXPButton	m_back;
	CXPButton	m_search;
	CXPButton	m_choose;
	CSortListCtrl	m_teachlist;
	CXPButton	m_classquery;
	CXPButton	m_showall;
	CXPButton	m_paperquery;
	CSortListCtrl	m_paperlist;
	CXPButton	m_ok;
	CSortListCtrl	m_classlist;
	CXPButton	m_add;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormPCoffRoll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormPCoffRoll)
	virtual BOOL OnInitDialog();
	afx_msg void OnClassquery();
	afx_msg void OnShowall();
	afx_msg void OnPaperquery();
	afx_msg void OnAdd();
	afx_msg void OnSearch();
	afx_msg void OnChoose();
	afx_msg void OnOk();
	afx_msg void OnBack();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int  m_step;
	BOOL b_isOpencom;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMPCOFFROLL_H__8EA64316_CA73_442E_A94B_B85FE79B181A__INCLUDED_)
