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
	std::list<CYDObjectRef*>		m_lstQuestionVault;//���е����
	HRESULT FillGrid();
	HRESULT InsertItem(CYDQuestionType* pType, 
						CYDObjectRef* _pVault,
						CBCGPGridRow* pParentRow);
	HRESULT InitQuestionVault();//��ʼ�����
	HRESULT UpdateQuestionCfg();//�ѱ�������Ŀ��Ϣ���뵽m_pSelQCfgMgr��
	//�������ͣ����ѳ̶Ⱥ�ÿ��Ŀ����������ѯm_pSelQCfgMgr�ж�Ӧ�������������û���ҵ�������_lNum=0
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
