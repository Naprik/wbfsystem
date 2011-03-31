#if !defined(AFX_FORMANSWER_H__4226E80F_140E_4384_A510_14060C825460__INCLUDED_)
#define AFX_FORMANSWER_H__4226E80F_140E_4384_A510_14060C825460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "ListCtrlComboBox.H"
#include "DBPaper.h"
#include "DBStdanswer.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"
#include "ControlButton.h"
// FormAnswer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormAnswer dialog

class CFormAnswer : public cdxCSizingDialog
{
// Construction
public:
	CFormAnswer(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormAnswer)
// Dialog Data
	//{{AFX_DATA(CFormAnswer)
	enum { IDD = IDD_FORM_ANSWER };
	CXPButton	m_update;
	CXPButton	m_showall;
	CXPButton	m_save;
	CXPButton	m_querypaper;
	CXPButton	m_delete;
	CXPButton	m_addanswer;
	CXPButton	m_add;
	CSortListCtrl	m_paperlist;
	CSortListCtrl	m_answerlist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormAnswer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrlComboBox m_ListComboBox;
	// Generated message map functions
	//{{AFX_MSG(CFormAnswer)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnUpdate();
	afx_msg void OnSave();
	afx_msg void OnDelete();
	afx_msg void OnClickStdanswerlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowall();
	afx_msg void OnDblclkPaperlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddanswer();
	afx_msg void OnQuerypaper();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString  GetExePath();
	CColorStatic m_Static1;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMANSWER_H__4226E80F_140E_4384_A510_14060C825460__INCLUDED_)
