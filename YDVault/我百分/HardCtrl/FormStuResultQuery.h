//{{AFX_INCLUDES()
#include "mschart.h"
//}}AFX_INCLUDES
#if !defined(AFX_FORMSTURESULTQUERY_H__4EB7EEAC_128D_432B_8D4D_0AC3A41E3214__INCLUDED_)
#define AFX_FORMSTURESULTQUERY_H__4EB7EEAC_128D_432B_8D4D_0AC3A41E3214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "ControlButton.h"
#include "XPButton.h"

class CFormStuResultQuery : public cdxCSizingDialog
{
// Construction
public:
	CFormStuResultQuery(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormStuResultQuery)

	void CreateBMPFile(HWND   hwnd,   LPTSTR   pszFile,   PBITMAPINFO   pbi,   HBITMAP   hBMP,   HDC   hDC);
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

// Dialog Data
	//{{AFX_DATA(CFormStuResultQuery)
	enum { IDD = IDD_FORM_STU_RESULT_QUERY };
	CSortListCtrl	m_stulist2;
	CXPButton	m_showchart;
	CXPButton	m_search;
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
	//{{AFX_VIRTUAL(CFormStuResultQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static void DrawInfo(CDC &memDC, PRNINFO PrnInfo);
	// Generated message map functions
	//{{AFX_MSG(CFormStuResultQuery)
	virtual BOOL OnInitDialog();
	afx_msg void OnQuery();
	afx_msg void OnSelchangeType();
	afx_msg void OnPrintReport();
	afx_msg void OnExportReport();
	afx_msg void OnPrintChart();
	afx_msg void OnExportChart();
	afx_msg void OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowchart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score);
	BOOL  showflag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMSTURESULTQUERY_H__4EB7EEAC_128D_432B_8D4D_0AC3A41E3214__INCLUDED_)
