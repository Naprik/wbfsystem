#if !defined(AFX_DLGCLASSSET_H__4F3FDACF_5DCF_4FE0_8904_E80AF85B849D__INCLUDED_)
#define AFX_DLGCLASSSET_H__4F3FDACF_5DCF_4FE0_8904_E80AF85B849D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgClassSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgClassSet dialog
#include "resource.h"

class CYDObjectRef;

class BASE_DLL_API CDlgClassSet : public CDialog
{
// Construction
public:
	CDlgClassSet(OPERATION op, CWnd* pParent = NULL);   // standard constructor
	CString  strDepart;
// Dialog Data
	//{{AFX_DATA(CDlgClassSet)
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_CLASS_SET };
	CString	m_classid;
	CString	m_class;
	CString	m_classheadtel;
	CString	m_classhead;
	OPERATION m_opMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgClassSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgClassSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strDepart;
	afx_msg void OnBnClickedOk();
	virtual INT_PTR DoModal();
	CYDObjectRef*	m_pDepart;
	CYDObjectRef*	m_pClass;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCLASSSET_H__4F3FDACF_5DCF_4FE0_8904_E80AF85B849D__INCLUDED_)
