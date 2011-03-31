#if !defined(AFX_DLGANSWERSET_H__DC5A7821_A812_41EF_86CC_704AFA3AA20B__INCLUDED_)
#define AFX_DLGANSWERSET_H__DC5A7821_A812_41EF_86CC_704AFA3AA20B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "ListCtrlComboBox.H"
#include "ListCtrlEdit.h"
// DlgAnswerset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAnswerset dialog

class CDlgAnswerset : public CDialog
{
// Construction
public:
	CDlgAnswerset(CWnd* pParent = NULL);   // standard constructor
	int startNO, endNO, paperID;

// Dialog Data
	//{{AFX_DATA(CDlgAnswerset)
	enum { IDD = IDD_DLG_ANSWER_SET };
	CListCtrl	m_list;
	CXPButton	m_ok;
	CXPButton	m_cancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnswerset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrlComboBox m_ListComboBox;
	CListCtrlEdit     m_Listedit;
	// Generated message map functions
	//{{AFX_MSG(CDlgAnswerset)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANSWERSET_H__DC5A7821_A812_41EF_86CC_704AFA3AA20B__INCLUDED_)
