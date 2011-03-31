#if !defined(AFX_FORMEXAMTIME_H__75AEFFAB_1294_4CF7_80D9_AE060D8E8017__INCLUDED_)
#define AFX_FORMEXAMTIME_H__75AEFFAB_1294_4CF7_80D9_AE060D8E8017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../uibase/ControlButton.h"
#include "Resource.h"
#include "../YDFormUIBase\YdFormView.h"
// FormExamTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamTime dialog

class CFormExamTime : public CYdFormView
{
// Construction
public:
	CFormExamTime();   // standard constructor
	DECLARE_DYNCREATE(CFormExamTime)

	void SetKind(int kind);
	int m_kind;    //进入item的编号，来自mainfram
	void SetInfo();
// Dialog Data
	//{{AFX_DATA(CFormExamTime)
	enum { IDD = IDD_YDUIEXAM_FORM_EXAM_TIME };
	CControlButton	m_up;
	CControlButton	m_next;
	CControlButton	m_change;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamTime)
	virtual void OnInitialUpdate();
	afx_msg void OnUp();
	afx_msg void OnNext();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMTIME_H__75AEFFAB_1294_4CF7_80D9_AE060D8E8017__INCLUDED_)
