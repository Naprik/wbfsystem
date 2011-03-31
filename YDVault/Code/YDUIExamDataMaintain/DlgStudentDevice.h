#if !defined(AFX_DLGSUBJECT_H__55B8AFEC_0221_4FB9_A16A_8ADF2232DDB5__INCLUDED_)
#define AFX_DLGSUBJECT_H__55B8AFEC_0221_4FB9_A16A_8ADF2232DDB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubject.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog

class BASE_DLL_API CDlgStudentDevice : public CDialog
{
// Construction
public:
	CDlgStudentDevice(OPERATION op, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSubject)
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_DEVICE};
	
	CString		m_strCode;
	CString		m_strMac;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlgSubject)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	OPERATION m_op;
public:
	virtual INT_PTR DoModal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBJECT_H__55B8AFEC_0221_4FB9_A16A_8ADF2232DDB5__INCLUDED_)
