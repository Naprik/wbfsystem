#if !defined(AFX_FORMEXAMSET_H__771C9EE8_6BE7_4674_9FF4_C4D237FDB75B__INCLUDED_)
#define AFX_FORMEXAMSET_H__771C9EE8_6BE7_4674_9FF4_C4D237FDB75B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBExamaddr.h"
#include "DBDepartment.h"
#include "ControlButton.h"
// FormExamSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamSet dialog

class CFormExamSet : public cdxCSizingDialog
{
// Construction
public:
	CFormExamSet(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExamSet)

	void SetInfo();
	void SetKind(int kind);
	int m_kind;    //进入item的编号，来自mainfram
// Dialog Data
	//{{AFX_DATA(CFormExamSet)
	enum { IDD = IDD_FORM_EXAMSET };
	CXPButton	m_up;
	CXPButton	m_showall;
	CXPButton	m_set;
	CXPButton	m_paperquery;
	CXPButton	m_next;
	CXPButton	m_addpaper;
	CSortListCtrl	m_paperlist;
	CSortListCtrl	m_classlist;
	CComboBox	m_address;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnUp();
	afx_msg void OnShowall();
	afx_msg void OnPaperquery();
	afx_msg void OnAddpaper();
	afx_msg void OnNext();
	afx_msg void OnDblclkPaperlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkClasslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMSET_H__771C9EE8_6BE7_4674_9FF4_C4D237FDB75B__INCLUDED_)
