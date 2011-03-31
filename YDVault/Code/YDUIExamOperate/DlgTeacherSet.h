#if !defined(AFX_DLGTEACHERSET_H__DDFF4A13_156C_4B6D_ACA5_D922DFDD25F0__INCLUDED_)
#define AFX_DLGTEACHERSET_H__DDFF4A13_156C_4B6D_ACA5_D922DFDD25F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTeacherSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherSet dialog
#include "Resource.h"

class CDlgTeacherSet : public CDialog
{
// Construction
public:
	CDlgTeacherSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTeacherSet)
	enum { IDD = IDD_YDUIEXAM_DLG_TEACHER_SET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTeacherSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTeacherSet)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEACHERSET_H__DDFF4A13_156C_4B6D_ACA5_D922DFDD25F0__INCLUDED_)
