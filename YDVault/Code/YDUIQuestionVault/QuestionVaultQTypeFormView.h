#pragma once
#include "../YDFormUIBase/YdFormView.h"
#include "afxcmn.h"
#include "Resource.h"

class CYDLinkRef;
class CYDObjectRef;
class CYDQuestionType;

// CQuestionVaultQTypeFormView form view

class  BASE_DLL_API CQuestionVaultQTypeFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CQuestionVaultQTypeFormView)

protected:
	CQuestionVaultQTypeFormView();           // protected constructor used by dynamic creation
	virtual ~CQuestionVaultQTypeFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_QUESTION_VAULT_QTYPE };
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
	CListCtrl m_lstQTypeExist;
	CListCtrl m_lstQTypeAll;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	virtual void ReadonlyPage(BOOL bReadonly);
private:
	std::list<CYDObjectRef*>   m_lstClearObj;
	std::list<CYDLinkRef*>     m_lstClearLink;
private:
	HRESULT InsertExistQType(CYDLinkRef* _pLinkRef);
	HRESULT InsertAllQType(CYDObjectRef* _pObjRef);
	HRESULT RepalceQType(CYDLinkRef* _pLinkRef,std::list<CYDQuestionType*> _lstAllQType);
public:
	afx_msg void OnBnClickedButtonFactorinfoConifg();
	afx_msg void OnBnClickedButtonVaultLevelConfig();
};


