#pragma once
#include "../uibase/BCGPKeyGridCtrl.h"
#include <list>

class CYDQuestionVault;
class CYDQuestionType;
class CYDObjectRef;

// CYDVaultFactorInfoConfig dialog

class CYDVaultFactorInfoConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CYDVaultFactorInfoConfig)

public:
	CYDVaultFactorInfoConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYDVaultFactorInfoConfig();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_FACTORINFO_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	CYDQuestionVault*	m_pVault;
private:
	CBCGPKeyGridCtrl						m_Grid;
	std::list<CYDQuestionType*>				m_lstType;
	std::list<CYDObjectRef*>				m_lstOldFactorInfoItem;//原来的配置映射关系对象
	HRESULT InsertByQuestionType(CYDQuestionType* _pQType);
};
