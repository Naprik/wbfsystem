//{{AFX_INCLUDES()
#include "mschart.h"
//}}AFX_INCLUDES
#if !defined(AFX_FORMCLASSRESULTQUEY_H__55369076_0B3E_4059_8303_99F2ECB15E6D__INCLUDED_)
#define AFX_FORMCLASSRESULTQUEY_H__55369076_0B3E_4059_8303_99F2ECB15E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "ControlButton.h"
#include "XPButton.h"
// FormClassResultQuey.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormClassResultQuey dialog

class CFormClassResultQuey : public cdxCSizingDialog
{
// Construction
public:
	CFormClassResultQuey(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormClassResultQuey)
// Dialog Data
	//{{AFX_DATA(CFormClassResultQuey)
	enum { IDD = IDD_FORM_CLASS_RESULT_QUERY };
	CXPButton	m_query;
	CXPButton	m_printreport;
	CXPButton	m_printchart;
	CXPButton	m_exportreport;
	CXPButton	m_exportchart;
	CComboBox	m_type;
	CSortListCtrl	m_stulist;
	CMSChart	m_chart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormClassResultQuey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormClassResultQuey)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuery();
	afx_msg void OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeType();
	afx_msg void OnPrintChart();
	afx_msg void OnPrintReport();
	afx_msg void OnExportReport();
	afx_msg void OnExportChart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score);
	int m_paperid;
	int m_startno;
	int m_endno;
	CString m_class;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMCLASSRESULTQUEY_H__55369076_0B3E_4059_8303_99F2ECB15E6D__INCLUDED_)
