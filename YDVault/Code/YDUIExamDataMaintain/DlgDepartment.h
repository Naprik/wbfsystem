#if !defined(AFX_DLGDPARTMENT_H__DF93AAFE_D6D3_4F2C_B318_E57AE1BD6523__INCLUDED_)
#define AFX_DLGDPARTMENT_H__DF93AAFE_D6D3_4F2C_B318_E57AE1BD6523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgDpartment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDepartment dialog
#include "Resource.h"
#include "../UIBase/XPButton.h"
class CYDObjectRef;
class BASE_DLL_API CDlgDepartment : public CDialog
{
// Construction
public:
	CDlgDepartment(OPERATION op, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDepartment)
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_DEPARTMENT };
	CString	m_head;
	CString	m_tel;
	CString	m_depart;
	CString	m_departid;
	OPERATION m_opMode;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDepartment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDepartment)
	virtual BOOL OnInitDialog();
//	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual INT_PTR DoModal();
	CYDObjectRef*	m_pDepart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDPARTMENT_H__DF93AAFE_D6D3_4F2C_B318_E57AE1BD6523__INCLUDED_)
