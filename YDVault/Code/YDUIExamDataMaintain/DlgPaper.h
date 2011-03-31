#if !defined(AFX_DLGSUBJECT_H__55B8AFEC_0221_4FB9_A16A_8ADF2232DDB5__INCLUDED_)
#define AFX_DLGSUBJECT_H__55B8AFEC_0221_4FB9_A16A_8ADF2232DDB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubject.h : header file
//
#include "resource.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog

class BASE_DLL_API CDlgPaper : public CDialog
{
// Construction
public:
	CDlgPaper(OPERATION op, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSubject)
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_PAPER };
	
	CString		m_strCode;
	CString		m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlgSubject)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual INT_PTR DoModal();
public:
	long m_uBegin1;
	long m_uEnd1;
	long m_uCount1;
	long m_uMuti1;

	long m_uBegin2;
	long m_uEnd2;
	long m_uCount2;
	long m_uMuti2;

	long m_uBegin3;
	long m_uEnd3;
	long m_uCount3;
	long m_uMuti3;

private:
	CComboBox m_cmbMuti1;
	CComboBox m_cmbMuti2;
	CComboBox m_cmbMuti3;

	OPERATION m_op;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBJECT_H__55B8AFEC_0221_4FB9_A16A_8ADF2232DDB5__INCLUDED_)
