#pragma once
#include "resource.h"

// CUserInfoDlg dialog
class CYDUserRef;
class CUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	HRESULT Validate(VARIANT_BOOL* _bValidate);

public:
	CString m_strUserName;
	CString m_strUserPwd;
	CComboBox m_gender;
	long m_iAge;
	CString m_strPhone;
	CString m_strMail;
	CString m_strID;
	CString m_strQQ;
	CString	m_strGender;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CYDUserRef*	m_pUser;
};
