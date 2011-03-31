#if !defined(AFX_FORMEXAMINFRAR_H__601D367F_F17D_4E1E_8E45_50D6F749A3DA__INCLUDED_)
#define AFX_FORMEXAMINFRAR_H__601D367F_F17D_4E1E_8E45_50D6F749A3DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../uibase/ControlButton.h"
#include "Resource.h"
#include "../YDFormUIBase\YdFormView.h"
#include "../UIBase\SortListCtrl.h"
// FormExamInfrar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfrar dialog

class BASE_DLL_API CFormExamInfrar : public CYdFormView
{
// Construction
public:
	CFormExamInfrar();   // standard constructor
	DECLARE_DYNCREATE(CFormExamInfrar)
	void SetInfo();
	void SetKind(int kind);
	int m_kind;		//进入item的编号，来自mainfram
// Dialog Data
	//{{AFX_DATA(CFormExamInfrar)
	enum { IDD = IDD_YDUIEXAM_FORM_EXAM_INFRAR };
	CControlButton	m_up;
	CControlButton	m_ok;
	CSortListCtrl	m_stulist;
	CString	m_systime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamInfrar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamInfrar)
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUp();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMINFRAR_H__601D367F_F17D_4E1E_8E45_50D6F749A3DA__INCLUDED_)
