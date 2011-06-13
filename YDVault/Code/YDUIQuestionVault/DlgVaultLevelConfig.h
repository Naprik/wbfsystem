#pragma once
class CYDObjectRef;
#include "../UIBase/BCGPKeyGridCtrl.h"

// CDlgVaultLevelConfig dialog

class CDlgVaultLevelConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVaultLevelConfig)

public:
	CDlgVaultLevelConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgVaultLevelConfig();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_VAULTLEVEL_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CBCGPKeyGridCtrl						m_Grid;
public:
	CYDObjectRef*							m_pVault;
	std::list<CYDObjectRef*>				m_lstVaultLevel;//原来已有的Level
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
private:
	HRESULT ValidateData(BOOL &_bValidate);
public:
	afx_msg void OnBnClickedOk();
};
