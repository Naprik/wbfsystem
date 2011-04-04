#pragma once
#include "../uibase/BCGPKeyGridCtrl.h"
#include <list>

class CYDQuestionVault;
class CYDQuestionType;
class CYDObjectRef;
class CDatabaseEx;

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
	HRESULT InsertRowByFactorInfoItem(CBCGPGridRow* pParentRow,CYDObjectRef* _pFactorInfoItem);
	HRESULT CreateRowFactorInfoItem(CBCGPGridRow* pParentRow,
									CBCGPGridRow* &_pChildRow);//创建一行，这一行是填入的配置关系的一些默认信息
	HRESULT DelOldItem();//将原来的配置关系删除
	HRESULT InsertItemByRowType(CBCGPGridRow* _pRowType,CDatabaseEx* pDb);
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
