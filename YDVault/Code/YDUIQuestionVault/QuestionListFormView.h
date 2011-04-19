#pragma once
#include "afxcmn.h"
#include "../YDFormUIBase/YdFormView.h"
#include "Resource.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

// CQuestionListFormView form view
class CYDQuestionVault;
class CYDQuestionType;
class CYDQuestionDlg;
class CListCtrlOperate;
class CPropQueryContidition;
class CYDQuestionRef;
class CYDLinkRef;


class BASE_DLL_API CQuestionListFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CQuestionListFormView)

protected:
	CQuestionListFormView();           // protected constructor used by dynamic creation
	virtual ~CQuestionListFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_QUESTIONLIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstQuestion;
	virtual void OnInitialUpdate();
private:
	//�õ���ǰ�����������������
	HRESULT GetQuestionVault(CYDQuestionVault* &_pQV);
	//�õ���ǰ����������
	HRESULT GetQuestionType(CYDQuestionType* &PpQType);
	HRESULT GetQuestionType(QTYPE* _pQtype);
	HRESULT GetQuestionTypeID(OBJID* _pUID);
	HRESULT CreateQuestionDlg(CYDQuestionDlg* &_pDlg);

	//�õ���ǰ���Ϳ��Բ�ѯ������������ʼ��ʱ����
	HRESULT CreateQuestionQueryProp();
	//�����б����ɲ�ѯ����
	HRESULT CreatePropQueryContidion();
	//��lstQuestion���뵽�б���
	HRESULT InsertlstToListCtrl(std::list<CYDQuestionRef*> &lstQuestion,std::list<CYDLinkRef*> &lstLink);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);//ִ�з�ҳ��ѯ
public:
	afx_msg void OnBnClickedButtonQlAdd();
	afx_msg void OnBnClickedButtonQlModify();
	afx_msg void OnBnClickedButtonQlDel();

	afx_msg void OnBnClickedButtonQlQueryAdd();
	afx_msg void OnBnClickedButtonQlQueryDel();
	afx_msg void OnBnClickedButtonQlQueryClear();
	afx_msg void OnBnClickedButtonQlQueryQuery();
	afx_msg void OnBnClickedButtonQlQueryBegin();
	afx_msg void OnBnClickedButtonQlQueryPrev();
	afx_msg void OnBnClickedButtonQlQueryNext();
	afx_msg void OnBnClickedButtonQlQueryEnd();

private:
//	CBCGPGridRow* AddSingGCLRow(const CYDQuestionRef& question, BOOL bNeedAdjustLayOut);
private:
	CBCGPKeyGridCtrl				m_gridConn;
	CListCtrlOperate*			m_plstOperate;
	std::list<CQueryProp*>      m_lstQueryProp;//��ǰ����������Բ�ѯ������
	UINT						m_uPage;//��ǰ��ʾ��ҳ��,ÿҳ��ʾQUESTION_PAGE_COUNT����¼
	std::list<CPropQueryContidition*> m_lstPropQuery;
	UINT						m_uTotalCount;//��ѯ����¼����
	UINT						m_uTotalPages;//��ѯ����ҳ��
public:
	afx_msg void OnNMDblclkListQlQuestion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonQlOutput();
	afx_msg void OnBnClickedButtonQlConfigFactorinfo();
	afx_msg void OnBnClickedButtonQlInput();
};


