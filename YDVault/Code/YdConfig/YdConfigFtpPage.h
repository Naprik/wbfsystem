#pragma once
class CFtpConfig;

// CYdConfigFtpPage dialog

class CYdConfigFtpPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CYdConfigFtpPage)

public:
	CYdConfigFtpPage();
	virtual ~CYdConfigFtpPage();

// Dialog Data
	enum { IDD = IDD_YDCONFIG_FTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
private:
	CFtpConfig*			m_pFtpConfig;
public:
	int m_iServerType;
	CString m_strFtpAddr; 
	CString m_strUser;
	CString m_strPwd;
	short m_sPort;
	afx_msg void OnBnClickedButtonTestConnet();
	BOOL	ValidateData();
	HRESULT UpdateFtpConfig(BOOL _bUpdate = TRUE);
	virtual BOOL OnInitDialog();
	virtual BOOL OnWizardFinish();
};
