#if !defined(AFX_DLGTEACHERDEV_H__0910CA6B_8836_4F44_BC19_20428F960FC8__INCLUDED_)
#define AFX_DLGTEACHERDEV_H__0910CA6B_8836_4F44_BC19_20428F960FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTeacherdev.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherdev dialog

class BASE_DLL_API CDlgTeacherdev : public CDialog
{
// Construction
public:
	CDlgTeacherdev(OPERATION op, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTeacherdev)
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_TEACHERDEV };
	CString	m_devmac;
	CString	m_devid;
	CString	m_teachername;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTeacherdev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTeacherdev)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	OPERATION m_opMode;
public:
	virtual INT_PTR DoModal();
private:
	BOOL	ValiateMac();//校验教师机地址格式
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEACHERDEV_H__0910CA6B_8836_4F44_BC19_20428F960FC8__INCLUDED_)
