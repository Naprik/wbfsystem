#if !defined(AFX_DLGEXAMEDIT_H__0A5A3A65_CB66_4EA7_A8F3_7A8400E5C249__INCLUDED_)
#define AFX_DLGEXAMEDIT_H__0A5A3A65_CB66_4EA7_A8F3_7A8400E5C249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExamEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExamEdit dialog

class CDlgExamEdit : public CDialog
{
// Construction
public:
	CDlgExamEdit(CWnd* pParent = NULL);   // standard constructor
	void init();
	BOOL decodeFM(CString &strFM);
// Dialog Data
	//{{AFX_DATA(CDlgExamEdit)
	enum { IDD = IDD_DLG_EXAM_EDIT };
	CButton	m_fm3;
	CButton	m_fm2;
	CButton	m_fm1;
	CString	m_channel;
	int		m_endno;
	CString	m_papername;
	int		m_startno;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExamEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExamEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTime();
	afx_msg void OnRadioYes();
	afx_msg void OnRadioNo();
	afx_msg void OnFm1();
	afx_msg void OnFm2();
	afx_msg void OnFm3();
	afx_msg void OnChangeFm1();
	afx_msg void OnChangeFm2();
	afx_msg void OnChangeFm3();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXAMEDIT_H__0A5A3A65_CB66_4EA7_A8F3_7A8400E5C249__INCLUDED_)
