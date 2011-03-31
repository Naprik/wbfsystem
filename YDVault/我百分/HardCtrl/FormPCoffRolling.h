#if !defined(AFX_FORMPCOFFROLLING_H__A78735E1_471D_402E_97D3_959B48117941__INCLUDED_)
#define AFX_FORMPCOFFROLLING_H__A78735E1_471D_402E_97D3_959B48117941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBStuInfo.h"
#include "DBResult.h"
// FormPCoffRolling.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormPCoffRolling dialog

class CFormPCoffRolling : public cdxCSizingDialog
{
// Construction
public:
	CFormPCoffRolling(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormPCoffRolling)

// Dialog Data
	//{{AFX_DATA(CFormPCoffRolling)
	enum { IDD = IDD_FORM_PCOFF_ROLLING };
	CXPButton	m_up;
	CXPButton	m_next;
	CSortListCtrl	m_teachlist;
	CXPButton	m_search;
	CXPButton	m_chose;
	CXPButton	m_show;
	CXPButton	m_save;
	CXPButton	m_query;
	CXPButton	m_fetch;
	CSortListCtrl	m_paperlist;
	CSortListCtrl	m_answerlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormPCoffRolling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormPCoffRolling)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnChose();
	afx_msg void OnQuery();
	afx_msg void OnShow();
	afx_msg void OnFetch();
	afx_msg void OnSave();
	afx_msg void OnUp();
	afx_msg void OnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int  m_step;
	BOOL b_isOpencom;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMPCOFFROLLING_H__A78735E1_471D_402E_97D3_959B48117941__INCLUDED_)
