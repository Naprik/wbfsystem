#if !defined(AFX_FORMEXAMINFOBACK_H__1F5DE402_2CF5_452A_B794_FC4E16B96D13__INCLUDED_)
#define AFX_FORMEXAMINFOBACK_H__1F5DE402_2CF5_452A_B794_FC4E16B96D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../uibase/ControlButton.h"
#include "Resource.h"
#include "../YDFormUIBase\YdFormView.h"
// FormExamInfoBack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfoBack dialog

class BASE_DLL_API CFormExamInfoBack : public CYdFormView
{
// Construction
public:
	CFormExamInfoBack();   // standard constructor
	DECLARE_DYNCREATE(CFormExamInfoBack)

	void SetKind(int kind);
	int m_kind;        //进入item的编号，来自mainfram；

// Dialog Data
	//{{AFX_DATA(CFormExamInfoBack)
	enum { IDD = IDD_YDUIEXAM_FORM_EXMA_INFO_BACK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamInfoBack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamInfoBack)
	afx_msg void OnNext();
	virtual void OnInitialUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMINFOBACK_H__1F5DE402_2CF5_452A_B794_FC4E16B96D13__INCLUDED_)
