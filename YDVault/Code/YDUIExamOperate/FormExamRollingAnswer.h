#if !defined(AFX_FORMEXAMROLLINGANSWER_H__C30AAF76_9024_4288_8DBB_AF82A41D1558__INCLUDED_)
#define AFX_FORMEXAMROLLINGANSWER_H__C30AAF76_9024_4288_8DBB_AF82A41D1558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// FormExamRollingAnswer.h : header file
//

#include "resource.h"
#include "../UIBase\SortListCtrl.h"
#include "../YDFormUIBase\YdFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CFormExamRollingAnswer dialog

class BASE_DLL_API CFormExamRollingAnswer :  public CYdFormView
{
// Construction
public:
	CFormExamRollingAnswer();   // standard constructor
	DECLARE_DYNCREATE(CFormExamRollingAnswer)
		
	HRESULT SetInfo();
	void SetKind(int kind);
	int  m_kind;		//进入item的编号，来自mainfram

// Dialog Data
	//{{AFX_DATA(CFormExamRollingAnswer)
	enum { IDD = IDD_YDUIEXAM_FORM_ROLLING_INRAR_ANSWER };
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamRollingAnswer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamRollingAnswer)
	virtual void OnInitialUpdate();
	afx_msg void OnFetchAnswer();
	afx_msg void OnSaveAnswer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMROLLINGANSWER_H__C30AAF76_9024_4288_8DBB_AF82A41D1558__INCLUDED_)
