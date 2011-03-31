#if !defined(AFX_DLGPAPERSET_H__7CEB51CB_1627_4356_A831_9E138DD53D3C__INCLUDED_)
#define AFX_DLGPAPERSET_H__7CEB51CB_1627_4356_A831_9E138DD53D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XPButton.h"
#include "DBDepartment.h"
#include "DBClass.h"
#include "DBSubject.h"
// DlgPaperset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperset dialog

class CDlgPaperset : public CDialog
{
// Construction
public:
	CDlgPaperset(CWnd* pParent = NULL);   // standard constructor
	CString strPaper;
	CString strDepart;
	CString strClass;
	CString strSubject;
	COleDateTime  tStart;
	COleDateTime  tEnd;
// Dialog Data
	//{{AFX_DATA(CDlgPaperset)
	enum { IDD = IDD_DLG_PAPER_SET };
	CXPButton	m_ok;
	CXPButton	m_cancel;
	CComboBox	m_subject;
	CComboBox	m_depart;
	CComboBox	m_class;
	int		m_endNO;
	COleDateTime	m_enddate;
	COleDateTime	m_endtime;
	int		m_maxNO;
	int		m_startNO;
	COleDateTime	m_startdate;
	COleDateTime	m_starttime;
	CString	m_papername;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPaperset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPaperset)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDepart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAPERSET_H__7CEB51CB_1627_4356_A831_9E138DD53D3C__INCLUDED_)
