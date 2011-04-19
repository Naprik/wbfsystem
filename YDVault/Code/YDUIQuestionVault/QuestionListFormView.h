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
	//得到当前试题类型所属的题库
	HRESULT GetQuestionVault(CYDQuestionVault* &_pQV);
	//得到当前的试题类型
	HRESULT GetQuestionType(CYDQuestionType* &PpQType);
	HRESULT GetQuestionType(QTYPE* _pQtype);
	HRESULT GetQuestionTypeID(OBJID* _pUID);
	HRESULT CreateQuestionDlg(CYDQuestionDlg* &_pDlg);

	//得到当前题型可以查询的属性名，初始化时调用
	HRESULT CreateQuestionQueryProp();
	//根据列表生成查询条件
	HRESULT CreatePropQueryContidion();
	//将lstQuestion插入到列表中
	HRESULT InsertlstToListCtrl(std::list<CYDQuestionRef*> &lstQuestion,std::list<CYDLinkRef*> &lstLink);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);//执行分页查询
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
	std::list<CQueryProp*>      m_lstQueryProp;//当前类型试题可以查询的属性
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*> m_lstPropQuery;
	UINT						m_uTotalCount;//查询到记录总数
	UINT						m_uTotalPages;//查询到总页数
public:
	afx_msg void OnNMDblclkListQlQuestion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonQlOutput();
	afx_msg void OnBnClickedButtonQlConfigFactorinfo();
	afx_msg void OnBnClickedButtonQlInput();
};


