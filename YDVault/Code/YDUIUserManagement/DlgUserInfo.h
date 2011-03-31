#pragma once


// CDlgUserInfo dialog

#include "resource.h"

class CYDObjectRef;

class CDlgUserInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserInfo)

public:
	CDlgUserInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUserInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	CString m_strUserPwd;
	BOOL m_bVault;
	BOOL m_bPaper;
	BOOL m_bOperate;
	BOOL m_bDataMaintain;
	OPERATION		m_TypeOperation;
	CYDObjectRef*	m_pUser;
private:
	HRESULT ReadOnlyControl(BOOL _bReadOnly);
	HRESULT Validate(VARIANT_BOOL* _bValidate);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckOperate();
	BOOL m_bBlueTooth;
	BOOL m_bRedOut;
};
