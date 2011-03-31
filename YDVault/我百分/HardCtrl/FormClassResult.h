#if !defined(AFX_FORMCLASSRESULT_H__38805E2C_EF5D_45C0_9835_241A27D9155C__INCLUDED_)
#define AFX_FORMCLASSRESULT_H__38805E2C_EF5D_45C0_9835_241A27D9155C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "FormGraphData.h"
#include "ControlButton.h"
//#include "FileGraph.h"
// FormClassResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormClassResult dialog

class CFormClassResult : public cdxCSizingDialog
{
// Construction
public:
	CFormClassResult(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormClassResult)
// Dialog Data
	//{{AFX_DATA(CFormClassResult)
	enum { IDD = IDD_FORM_CLASSRESULT };
	CButton	m_check;
	CSortListCtrl	m_classreslist2;
	CComboBox	m_gratype;
	CControlButton	m_print;
	CControlButton	m_export;
	CComboBox	m_type;
	CControlButton	m_sata;
	CControlButton	m_query;
	CSortListCtrl	m_classreslist;
	BOOL	m_isline;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormClassResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static void DrawInfo(CDC &memDC, PRNINFO PrnInfo);

	HICON m_hIcon;
//	CFileGraph  m_graphData;
	// Generated message map functions
	//{{AFX_MSG(CFormClassResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuery();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSata();
	afx_msg void OnExport();
	afx_msg void OnPrint();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeGraphType();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeType();
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score);
	CString  GetExePath();
	CColorStatic m_Static1;
	CColorStatic m_Static2;
	CFormGraphData  m_graph;
	CFileGraph  m_graphData;
	void    ShowGraph();
	BOOL	b_stat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMCLASSRESULT_H__38805E2C_EF5D_45C0_9835_241A27D9155C__INCLUDED_)
