#if !defined(AFX_DLGPEROLLING_H__700E0EF4_E21C_4893_A5C0_0DB1C7C2160B__INCLUDED_)
#define AFX_DLGPEROLLING_H__700E0EF4_E21C_4893_A5C0_0DB1C7C2160B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
#include "Stusearch.h"
// DlgPerolling.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPerolling dialog

class CDlgPerolling : public CDialog
{
// Construction
public:
	CDlgPerolling(CWnd* pParent = NULL);   // standard constructor
	CString m_subject;
	CString m_address;
	COleDateTime m_start;
	CStringArray    m_stulist;

	CString   m_strTemp;
	int		  m_iTemp;
	int       iPeperid;
	int       iStart;
	int		  iEnd;
	CString   strAdd;

	CArray<CStusearch, CStusearch> m_arryvalid;

// Dialog Data
	//{{AFX_DATA(CDlgPerolling)
	enum { IDD = IDD_DIG_PERROLLING };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CXPButton	m_search;
	CSortListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPerolling)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPerolling)
	afx_msg void OnSearch();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPEROLLING_H__700E0EF4_E21C_4893_A5C0_0DB1C7C2160B__INCLUDED_)
