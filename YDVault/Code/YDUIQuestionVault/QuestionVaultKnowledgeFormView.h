#pragma once
#include "../YDFormUIBase/YdFormView.h"
#include "afxcmn.h"
#include "Resource.h"


// CQuestionVaultKnowledgeFormView form view

class BASE_DLL_API CQuestionVaultKnowledgeFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CQuestionVaultKnowledgeFormView)

protected:
	CQuestionVaultKnowledgeFormView();           // protected constructor used by dynamic creation
	virtual ~CQuestionVaultKnowledgeFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_QUESTION_VAULT_KNOWLEDGE };
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
	virtual void OnInitialUpdate();
	CListCtrl m_lstKnowledge;
public:
	virtual void ReadonlyPage(BOOL bReadonly);
	COleDateTime m_dtCreate;
	afx_msg void OnLvnItemchangedListKnowledge(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDel();
private:
	HRESULT InsertObj(CYdObjWrapper* _pWrapper);
	HRESULT UpdateObj(int _index,CYdObjWrapper* _pWrapper);
private:
	std::list<CYdObjWrapper*>     m_lstObjWrapper;
public:
	afx_msg void OnNMDblclkListKnowledge(NMHDR *pNMHDR, LRESULT *pResult);
};


