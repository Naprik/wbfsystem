#if !defined(AFX_DLGROLLINGCHOOSE_H__82E95B0C_3634_440B_8B0D_33CBB9D47074__INCLUDED_)
#define AFX_DLGROLLINGCHOOSE_H__82E95B0C_3634_440B_8B0D_33CBB9D47074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRollingChoose.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRollingChoose dialog

class CDlgRollingChoose : public CDialog
{
// Construction
public:
	CDlgRollingChoose(CWnd* pParent = NULL);   // standard constructor
	int m_rollkind;
// Dialog Data
	//{{AFX_DATA(CDlgRollingChoose)
	enum { IDD = IDD_DLG_ROLLING_CHOOSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRollingChoose)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRollingChoose)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGROLLINGCHOOSE_H__82E95B0C_3634_440B_8B0D_33CBB9D47074__INCLUDED_)
