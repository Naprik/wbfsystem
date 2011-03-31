#if !defined(AFX_DLGMSG_H__9A440AA9_1B16_41AA_A821_A2C00ACC5F24__INCLUDED_)
#define AFX_DLGMSG_H__9A440AA9_1B16_41AA_A821_A2C00ACC5F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
class CDlgMsg : public cdxCSizingDialog
{
public:
	void ShowData(COleDateTime dt,CString domain);
	void ShowMsg(LPCTSTR lpszFormat, ...);
	CDlgMsg(CWnd* pParent = NULL);  
    DECLARE_DYNCREATE(CDlgMsg)
	//{{AFX_DATA(CDlgMsg)
	enum { IDD = IDD_DLG_MSG };
	CListBox	m_msg;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CDlgMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CDlgMsg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DLGMSG_H__9A440AA9_1B16_41AA_A821_A2C00ACC5F24__INCLUDED_)
