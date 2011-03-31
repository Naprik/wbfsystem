//{{AFX_INCLUDES()
#include "mschart.h"
//}}AFX_INCLUDES
#if !defined(AFX_FORMPAPERANALY_H__A94D8B50_0B55_4106_9DDB_0AA5F3CCD89F__INCLUDED_)
#define AFX_FORMPAPERANALY_H__A94D8B50_0B55_4106_9DDB_0AA5F3CCD89F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "Answer.h"
#include "ControlButton.h"
// FormPaperAnaly.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormPaperAnaly dialog

class CFormPaperAnaly : public cdxCSizingDialog
{
// Construction
public:
	CFormPaperAnaly(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormPaperAnaly)

	int m_paperid;
	int m_maxno;
	int m_startno;
	int m_endno;
	CString m_stdanswer;
	CString m_papername;
	void CreateBMPFile(HWND   hwnd,   LPTSTR   pszFile,   PBITMAPINFO   pbi,   HBITMAP   hBMP,   HDC   hDC);
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
// Dialog Data
	//{{AFX_DATA(CFormPaperAnaly)
	enum { IDD = IDD_FORM_PAPER_ANALY };
	CXPButton	m_query;
	CXPButton	m_printreport;
	CXPButton	m_printchart;
	CXPButton	m_exportreport;
	CXPButton	m_exportchart;
	CComboBox	m_type;
	CSortListCtrl	m_paperlist;
	CMSChart	m_chart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormPaperAnaly)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormPaperAnaly)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuery();
	afx_msg void OnSelchangeType();
	afx_msg void OnPrintChart();
	afx_msg void OnPrintreport();
	afx_msg void OnExportreport();
	afx_msg void OnExportChart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void sortarray(CArray<CAnswer, CAnswer>  &arryAnswer);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMPAPERANALY_H__A94D8B50_0B55_4106_9DDB_0AA5F3CCD89F__INCLUDED_)
