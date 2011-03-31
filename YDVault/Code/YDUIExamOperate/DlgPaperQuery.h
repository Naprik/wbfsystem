#if !defined(AFX_DLGPAPERQUERY_H__8335E34B_EB04_4032_88B9_5F0879ABBAAE__INCLUDED_)
#define AFX_DLGPAPERQUERY_H__8335E34B_EB04_4032_88B9_5F0879ABBAAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPaperQuery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperQuery dialog
#include "Resource.h"
class CYDObjectRef;

class CDlgPaperQuery : public CDialog
{
// Construction
public:
	CDlgPaperQuery(CWnd* pParent = NULL);   // standard constructor
	virtual	~CDlgPaperQuery();
	CString	 strDepart;
	CString  strSubject;
	CString  strClass;
	CString  strPaper;
	BOOL	 bIstime;
// Dialog Data
	//{{AFX_DATA(CDlgPaperQuery)
	enum { IDD = IDD_YDUIEXAM_DLG_PAPER_QUERY };
	CComboBox	m_paper;
	CButton	m_checktime;
	CButton	m_checkdepart;
	CDateTimeCtrl	m_timectl;
	CButton	m_checksub;
	CComboBox	m_subject;
	CComboBox	m_depart;
	CComboBox	m_class;
	COleDateTime	m_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPaperQuery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPaperQuery)
	afx_msg void OnChecksubject();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckdepart();
	afx_msg void OnChecktime();
	afx_msg void OnSelchangeDepart();
	virtual void OnOK();
	afx_msg void OnSelchangeSubject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	std::list<CYDObjectRef*> m_lstDepart;
	std::list<CYDObjectRef*> m_lstSubject;
	HRESULT Clear();
public:
	virtual INT_PTR DoModal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAPERQUERY_H__8335E34B_EB04_4032_88B9_5F0879ABBAAE__INCLUDED_)
