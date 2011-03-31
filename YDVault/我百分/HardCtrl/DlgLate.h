#if !defined(AFX_DLGLATE_H__D6E1E608_58F4_433E_ADF0_22EB9A26B6FF__INCLUDED_)
#define AFX_DLGLATE_H__D6E1E608_58F4_433E_ADF0_22EB9A26B6FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "cdxCSizingDialog.h"
#include "Stusearch.h"
// DlgLate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLate dialog

class CDlgLate : public CDialog
{
// Construction
public:
	CDlgLate(CWnd* pParent = NULL);   // standard constructor
	COleDateTime 	m_endtime;
	int	            m_maxNo;
	CString			m_subject;
	CString		    m_address;
	CStringArray    m_list;

	CString         m_strTemp;
	int				m_iTemp;
	COleDateTime    m_start;

	CArray<CStusearch, CStusearch> m_arryvalid;
// Dialog Data
	//{{AFX_DATA(CDlgLate)
	enum { IDD = IDD_DLG_LATE };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CXPButton	m_latestart;
	CXPButton	m_latesearch;
	CSortListCtrl	m_latelist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLate)
	afx_msg void OnLatesearch();
	afx_msg void OnLatestart();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLatelist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLATE_H__D6E1E608_58F4_433E_ADF0_22EB9A26B6FF__INCLUDED_)
