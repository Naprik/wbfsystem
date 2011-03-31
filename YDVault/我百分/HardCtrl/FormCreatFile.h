#if !defined(AFX_FORMCREATFILE_H__C075790D_492E_49D0_885F_600AB9814B30__INCLUDED_)
#define AFX_FORMCREATFILE_H__C075790D_492E_49D0_885F_600AB9814B30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
// FormCreatFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormCreatFile dialog

class CFormCreatFile : public cdxCSizingDialog
{
// Construction
public:
	CFormCreatFile(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormCreatFile)

	void SetInfo();
	void SetKind(int kind);
	int m_kind;		//进入item的编号，来自mainfram
// Dialog Data
	//{{AFX_DATA(CFormCreatFile)
	enum { IDD = IDD_FORM_CREAT_FILE };
	CSortListCtrl	m_list;
	CXPButton	m_create;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormCreatFile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormCreatFile)
	virtual BOOL OnInitDialog();
	afx_msg void OnCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMCREATFILE_H__C075790D_492E_49D0_885F_600AB9814B30__INCLUDED_)
