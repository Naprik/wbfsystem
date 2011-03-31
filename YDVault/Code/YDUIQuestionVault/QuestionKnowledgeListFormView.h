#pragma once
#include "../YDFormUIBase/YdFormView.h"
#include "afxcmn.h"
#include "Resource.h"

// CQuestionKnowledgeListFormView form view
class CYdKnowledge;
class BASE_DLL_API CQuestionKnowledgeListFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CQuestionKnowledgeListFormView)

protected:
	CQuestionKnowledgeListFormView();           // protected constructor used by dynamic creation
	virtual ~CQuestionKnowledgeListFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_QUESTION_KNOWLEDGE };
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
	virtual void ReadonlyPage(BOOL bReadonly);
	virtual HRESULT UpdateProp(BOOL bUpdate) ;

	virtual void OnInitialUpdate();
	CListCtrl m_lstAllKnowledge;
	CListCtrl m_lstRelateKnowledge;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	virtual HRESULT Save();
private:
	std::list<CYdKnowledge*> m_lstKnowledgeRef;
};


