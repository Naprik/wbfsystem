#pragma once
#include "resource.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../include\extdllstate.h"


// CQuestionVaultClassificationFormView form view

class BASE_DLL_API CQuestionVaultClassificationFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CQuestionVaultClassificationFormView)

protected:
	CQuestionVaultClassificationFormView();           // protected constructor used by dynamic creation
	virtual ~CQuestionVaultClassificationFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_QUESTION_VAULT_CLASSICATION };
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
public:
	virtual void ReadonlyPage(BOOL bReadonly);
	virtual HRESULT UpdateProp(BOOL bUpdate) ;
	BOOL	ValidateData(OPERATION op);
	CString m_strName;
	CString m_strCreater;
	CString m_strDesc;
	CString m_strCreateDate;
};


