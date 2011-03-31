#pragma once


// CDlgModifyPwd dialog
#include "Resource.h"

class AFX_EXT_CLASS CDlgModifyPwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyPwd)

public:
	CDlgModifyPwd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModifyPwd();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODIFY_PWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strOldPwd;
	CString m_strNewPwd;
	CString m_strNewPwdConfirm;
};
