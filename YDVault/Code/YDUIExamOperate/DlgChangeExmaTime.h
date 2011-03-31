#if !defined(AFX_DLGCHANGEEXMATIME_H__25ECF26D_D60A_4E8A_BF15_AA4DC3E49CE2__INCLUDED_)
#define AFX_DLGCHANGEEXMATIME_H__25ECF26D_D60A_4E8A_BF15_AA4DC3E49CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChangeExmaTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeExmaTime dialog
#include "Resource.h"
class CDlgChangeExmaTime : public CDialog
{
// Construction
public:
	CDlgChangeExmaTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChangeExmaTime)
	enum { IDD = IDD_YDUIEXAM_DLG_CHANGE_EXAM_TIME };
	COleDateTime	m_enftime;
	COleDateTime	m_starttime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChangeExmaTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChangeExmaTime)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANGEEXMATIME_H__25ECF26D_D60A_4E8A_BF15_AA4DC3E49CE2__INCLUDED_)
