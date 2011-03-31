#if !defined(AFX_FORMTEACHERDEV_H__BCBB7D88_35D5_46CD_96DD_D5090DFDE3B7__INCLUDED_)
#define AFX_FORMTEACHERDEV_H__BCBB7D88_35D5_46CD_96DD_D5090DFDE3B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBTeacherDev.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// FormTeacherDev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormTeacherDev dialog

class CFormTeacherDev : public cdxCSizingDialog
{
// Construction
public:
	CFormTeacherDev(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormTeacherDev)
// Dialog Data
	//{{AFX_DATA(CFormTeacherDev)
	enum { IDD = IDD_FORM_TEACHERDEV };
	CXPButton	m_update;
	CXPButton	m_showall;
	CXPButton	m_delete;
	CXPButton	m_add;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormTeacherDev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void		 showdata();
	// Generated message map functions
	//{{AFX_MSG(CFormTeacherDev)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnUpdate();
	afx_msg void OnShowall();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMTEACHERDEV_H__BCBB7D88_35D5_46CD_96DD_D5090DFDE3B7__INCLUDED_)
