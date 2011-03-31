#if !defined(AFX_BELOWBAR_H__E7C387A5_C39A_4E2C_861F_C29079A8A9A3__INCLUDED_)
#define AFX_BELOWBAR_H__E7C387A5_C39A_4E2C_861F_C29079A8A9A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BelowBar.h : header file
//
//#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CBelowBar window

class CBelowBar : public CDialogBar
{
// Construction
public:
	CBelowBar();

// Attributes
public:

// Operations
public:
	//这两个方法主要用于设置按钮
	void ShowBegin(OPERATION type);
	void SaveCompleted();
	void UpdateCompleted();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBelowBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBelowBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBelowBar)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BELOWBAR_H__E7C387A5_C39A_4E2C_861F_C29079A8A9A3__INCLUDED_)
