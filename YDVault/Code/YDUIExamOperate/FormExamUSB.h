#if !defined(AFX_FORMEXAMUSB_H__3B7F8A92_A662_4794_8BBB_7A52AAB26C96__INCLUDED_)
#define AFX_FORMEXAMUSB_H__3B7F8A92_A662_4794_8BBB_7A52AAB26C96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../uibase/ControlButton.h"
#include "resource.h"
#include "../UIBase\SortListCtrl.h"
#include "../YDFormUIBase\YdFormView.h"
// FormExamUSB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamUSB dialog

class BASE_DLL_API CFormExamUSB : public CYdFormView
{
// Construction
public:
	CFormExamUSB();   // standard constructor
	DECLARE_DYNCREATE(CFormExamUSB)

	void SetInfo();
	void SetKind(int kind);
	int m_kind;		//进入item的编号，来自mainfram
// Dialog Data
	//{{AFX_DATA(CFormExamUSB)
	enum { IDD = IDD_YDUIEXAM_FORM_EXAM_USB };
	CControlButton	m_search;
	CControlButton	m_roll;
	CSortListCtrl	m_stulist;
	CString	m_systime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamUSB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamUSB)
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSearch();
	afx_msg void OnRollcall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMUSB_H__3B7F8A92_A662_4794_8BBB_7A52AAB26C96__INCLUDED_)
