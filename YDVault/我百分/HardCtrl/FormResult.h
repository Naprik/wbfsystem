#if !defined(AFX_FORMRESULT_H__0AEC0E17_CA94_4B57_B44E_B7A559835042__INCLUDED_)
#define AFX_FORMRESULT_H__0AEC0E17_CA94_4B57_B44E_B7A559835042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBResult.h"
#include "DBStuInfo.h"
#include "DBPaper.h"
#include "DBStdanswer.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
// FormResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormResult dialog

class CFormResult : public cdxCSizingDialog
{
// Construction
public:
	CFormResult(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormResult)

// Dialog Data
	//{{AFX_DATA(CFormResult)
	enum { IDD = IDD_FORM_RESULT };
	CShadeButtonST	m_export;
	CShadeButtonST	m_print;
	CShadeButtonST	m_stat;
	CComboBox	m_type;
	CShadeButtonST	m_query;
	CSortListCtrl	m_resultlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static void DrawInfo(CDC &memDC, PRNINFO PrnInfo);

	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CFormResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuery();
	afx_msg void OnDblclkResultlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSata();
	afx_msg void OnPrint();
	afx_msg void OnExport();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score);
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMRESULT_H__0AEC0E17_CA94_4B57_B44E_B7A559835042__INCLUDED_)
