#if !defined(AFX_DLGSTUQUERY_H__908679AF_65E4_4AE6_B7AC_854135224091__INCLUDED_)
#define AFX_DLGSTUQUERY_H__908679AF_65E4_4AE6_B7AC_854135224091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStuQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStuQuery dialog

class CDlgStuQuery : public CDialog
{
// Construction
public:
	CDlgStuQuery(CWnd* pParent = NULL);   // standard constructor
	CString strClass;
// Dialog Data
	//{{AFX_DATA(CDlgStuQuery)
	enum { IDD = IDD_DLG_QUERY };
	CComboBox	m_class;
	CEdit	m_name;
	CEdit	m_id;
	CString	m_stuid;
	CString	m_stuname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStuQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStuQuery)
	afx_msg void OnRadiostuid();
	afx_msg void OnRadiostuname();
	afx_msg void OnRadioclass();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTUQUERY_H__908679AF_65E4_4AE6_B7AC_854135224091__INCLUDED_)
