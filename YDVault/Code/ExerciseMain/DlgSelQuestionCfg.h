#pragma once
#include "afxwin.h"
#include "resource.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

class CQuestionCfgStruct;
class CSelectQuestionCfgMgr;
class CYDQuestionType;
class CYDQuestionVault;
class CYDObjectRef;

class CDlgSelQuestionCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelQuestionCfg)

public:
	CDlgSelQuestionCfg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelQuestionCfg();

// Dialog Data
	enum { IDD = IDD_DLG_EM_CFG_QUESTION_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCmbQvault();
	afx_msg void OnCbnSelchangeCmbQtype();

	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnUpdate();
	afx_msg void OnBnClickedBtnAdd();

	virtual BOOL OnInitDialog();

private:
	HRESULT InitQuestionVault();
	HRESULT InsertCondition(CQuestionCfgStruct* pCondition);
public:
	CSelectQuestionCfgMgr*			m_pSelQCfgMgr;
private:
	std::list<CYDObjectRef*>		m_lstQuestionVault;//所有的题库
	std::list<CYDQuestionType*>		m_lstClean;
	std::list<CQuestionCfgStruct*>  m_lstCfg;
	std::list<CYDObjectRef*>		m_lstfactor;
	CYDQuestionVault*				m_pVault;
	CBCGPKeyGridCtrl				m_gridFactors;
	CBCGPKeyGridCtrl				m_gridConditions;
	CComboBox						m_cmbQuestionVault;
	CComboBox						m_cmbQType;
	int m_qnum;
	double m_mark;
public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
