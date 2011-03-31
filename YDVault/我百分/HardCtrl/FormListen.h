#if !defined(AFX_FORMLISTEN_H__A4B0BAD4_4FC4_45B1_BD55_74701F5362C2__INCLUDED_)
#define AFX_FORMLISTEN_H__A4B0BAD4_4FC4_45B1_BD55_74701F5362C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "ControlButton.h"
// FormListen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormListen dialog

class CFormListen : public cdxCSizingDialog
{
// Construction
public:
	CFormListen(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormListen)

	void SetKind(int kind);
	int m_kind;      //进入item的编号，来自mainfram
	BOOL decodeFM(CString &strFM);
// Dialog Data
	//{{AFX_DATA(CFormListen)
	enum { IDD = IDD_FORM_LISTEN };
	CXPButton	m_next;
	CXPButton	m_up;
	CXPButton	m_change;
	CDateTimeCtrl	m_start3ctl;
	CDateTimeCtrl	m_start2ctl;
	CDateTimeCtrl	m_start1ctl;
	CDateTimeCtrl	m_end3ctl;
	CDateTimeCtrl	m_end2ctl;
	CDateTimeCtrl	m_end1ctl;
	CButton	m_check3;
	CButton	m_check2;
	CButton	m_check1;
	COleDateTime	m_end1;
	COleDateTime	m_end2;
	COleDateTime	m_end3;
	CString	m_systime;
	COleDateTime	m_start1;
	COleDateTime	m_start2;
	COleDateTime	m_start3;
	CString	m_fm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormListen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormListen)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRadioyes();
	afx_msg void OnRadiono();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnChangesystime();
	afx_msg void OnUp();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMLISTEN_H__A4B0BAD4_4FC4_45B1_BD55_74701F5362C2__INCLUDED_)
