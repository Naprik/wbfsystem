#pragma once
class CDBConfig;

// CYdConfigDBPage dialog

class CYdConfigDBPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CYdConfigDBPage)

public:
	CYdConfigDBPage();
	virtual ~CYdConfigDBPage();

// Dialog Data
	enum { IDD = IDD_YDCONFIG_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
private:
	CDBConfig*	m_pDBConfig;
public:
	virtual BOOL OnInitDialog();
	int m_iDBType;
	int m_iConnectionMode;
	CString m_strDBServerName;
	CString m_strUserName;
	CString m_strPwd;
	short m_sConnectTime;
	virtual LRESULT OnWizardNext();
	BOOL   ValidateData();
	afx_msg void OnBnClickedButtonTestConnect();
	HRESULT UpdateDBConfig(BOOL _bUpdate = TRUE);
	afx_msg void OnBnClickedButtonSelDbfile();
	CString m_strDBFile;
};
