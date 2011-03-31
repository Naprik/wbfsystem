#pragma once
#include "afxwin.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

class CSelectQuestionCfgMgr;
class CYDObjectRef;

// CDlgCfgQuestion dialog
class CYDQuestionType;
class CYDPaperBlockConfigure;
class CDlgCfgQuestion : public CDialog
{
	DECLARE_DYNAMIC(CDlgCfgQuestion)

public:
	CDlgCfgQuestion(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCfgQuestion();

// Dialog Data
	enum { IDD = IDD_DLG_EM_CFG_QUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSelectQuestionCfgMgr* m_pSelQCfgMgr;
	virtual BOOL OnInitDialog();
private:
	CBCGPKeyGridCtrl		m_gridList;
	std::list<CYDQuestionType*>		m_lstClean;
	std::list<CYDObjectRef*>		m_lstQuestionVault;//所有的题库
	HRESULT FillGrid();
	HRESULT InsertItem(CYDQuestionType* pType, 
						CYDObjectRef* _pVault,
						CBCGPGridRow* pParentRow);
	HRESULT InitQuestionVault();//初始化题库
	HRESULT UpdateQuestionCfg();//把表格填的题目信息填入到m_pSelQCfgMgr中
	//根据题型，困难程度和每题目问题数，查询m_pSelQCfgMgr中对应的问题数，如果没有找到，返回_lNum=0
	HRESULT GetQNum(CYDQuestionType* _pType,
					HARDLEVEL _level,
					UINT      _uEachNum,
					long &_lNum,
					double &_fMark);
	std::list<CYDPaperBlockConfigure*> m_lstCfg;
public:
	CComboBox m_cmbQuestionVault;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboQuestionVault();
};
