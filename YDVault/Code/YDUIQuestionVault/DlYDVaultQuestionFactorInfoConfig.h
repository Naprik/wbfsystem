#pragma once
#include "../uibase/BCGPKeyGridCtrl.h"
#include <list>

class CYDObjectRef;
// CDlYDVaultQuestionFactorInfoConfig dialog

class CDlYDVaultQuestionFactorInfoConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlYDVaultQuestionFactorInfoConfig)

public:
	CDlYDVaultQuestionFactorInfoConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlYDVaultQuestionFactorInfoConfig();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALG_QUESTION_FACTORINFO_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPKeyGridCtrl						m_Grid;
public:
	CYDObjectRef*		m_pVault;
	CYDObjectRef*		m_pQType;
	CYDObjectRef*		m_pQuestion;
	std::list<CYDObjectRef*>		m_lstFactorInfoItem;
	afx_msg void OnBnClickedOk();
};
