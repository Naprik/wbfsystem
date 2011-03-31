#if !defined(AFX_FORMSTUINFOSET_H__E6882A58_27E2_4114_ADB5_166C8EC4C3A4__INCLUDED_)
#define AFX_FORMSTUINFOSET_H__E6882A58_27E2_4114_ADB5_166C8EC4C3A4__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBDepartment.h"
#include "DBStuInfo.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// FormStuinfoSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormStuinfoSet dialog
class CFormStuinfoSet : public cdxCSizingDialog
{
// Construction
public:
	CFormStuinfoSet(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormStuinfoSet)

// Dialog Data
	//{{AFX_DATA(CFormStuinfoSet)
	enum { IDD = IDD_FORM_STUINFO_SET };
	CXPButton	m_update;
	CXPButton	m_querystud;
	CXPButton	m_delete;
	CXPButton	m_add;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormStuinfoSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormStuinfoSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuerystud();
	afx_msg void OnAddstud();
	afx_msg void OnDeletestud();
	afx_msg void OnUpdatestud();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
private:
	void  shownew(CString strStuID);
	void  refurbish();
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMSTUINFOSET_H__E6882A58_27E2_4114_ADB5_166C8EC4C3A4__INCLUDED_)
