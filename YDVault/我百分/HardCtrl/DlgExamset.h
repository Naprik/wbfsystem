#if !defined(AFX_DLGEXAMSET_H__5978F6C2_0E05_41C5_A357_5EB384D196D0__INCLUDED_)
#define AFX_DLGEXAMSET_H__5978F6C2_0E05_41C5_A357_5EB384D196D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "DBPaper.h"
#include "DBExamaddr.h"
#include "DBDepartment.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
// DlgExamset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExamset dialog

class CDlgExamset : public CDialog
{
// Construction
public:
	CDlgExamset(CWnd* pParent = NULL);   // standard constructor

	CString sPaper,sSubject,sDepart,sClass,sStarttime,sEndtime,sStartNO,sEndNO,sMaxNO;
	CString sAddr;
	CStringArray  classarry;
	int	paperid;

	CString GetExePath();
// Dialog Data
	//{{AFX_DATA(CDlgExamset)
	enum { IDD = IDD_DLG_EXAM_SET };
	CSortListCtrl	m_classlist;
	CComboBox	m_address;
	CShadeButtonST	m_ok;
	CShadeButtonST	m_cancel;
	CShadeButtonST	m_showall;
	CShadeButtonST	m_query;
	CSortListCtrl	m_list;
	CShadeButtonST	m_add;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExamset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExamset)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowall();
	afx_msg void OnQuerypaper();
	afx_msg void OnAddpaper();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CColorStatic m_Static1;
	CColorStatic m_Static2;
	CColorStatic m_Static3;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXAMSET_H__5978F6C2_0E05_41C5_A357_5EB384D196D0__INCLUDED_)
