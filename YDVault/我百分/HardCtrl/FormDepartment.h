#if !defined(AFX_FORMDEPARTMENT_H__4AB1430B_9B84_4AD4_B781_C1917DF4033F__INCLUDED_)
#define AFX_FORMDEPARTMENT_H__4AB1430B_9B84_4AD4_B781_C1917DF4033F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBDepartment.h"
#include "DBClass.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"

class CFormDepartment : public cdxCSizingDialog
{
// Construction
public:
	CFormDepartment(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormDepartment)
// Dialog Data
	//{{AFX_DATA(CFormDepartment)
	enum { IDD = IDD_FORM_DEPARTMENT };
	CXPButton	m_updatedepart;
	CXPButton	m_updateclass;
	CXPButton	m_display;
	CXPButton	m_deletedepart;
	CXPButton	m_deleteclass;
	CXPButton	m_adddepart;
	CXPButton	m_addclass;
	CSortListCtrl	m_classlist;
	CSortListCtrl	m_departlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDepartment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
// Implementation
protected:
	void showdata();
	// Generated message map functions
	//{{AFX_MSG(CFormDepartment)
	virtual BOOL OnInitDialog();
	afx_msg void OnDisplay();
	afx_msg void OnAdddepart();
	afx_msg void OnAddclass();
	afx_msg void OnDblclkDepartmentlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdatedepart();
	afx_msg void OnUpdateclass();
	afx_msg void OnDeletedepart();
	afx_msg void OnDeleteclass();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMDEPARTMENT_H__4AB1430B_9B84_4AD4_B781_C1917DF4033F__INCLUDED_)
