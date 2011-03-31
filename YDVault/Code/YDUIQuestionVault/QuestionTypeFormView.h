#pragma once

#include "../YDFormUIBase/YdFormView.h"
#include "Resource.h"

// CQuestionTypeFormView form view

class BASE_DLL_API CQuestionTypeFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CQuestionTypeFormView)


protected:
	CQuestionTypeFormView();           // protected constructor used by dynamic creation
	virtual ~CQuestionTypeFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_QUESTION_TYPE };
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
	CString m_strName;
	CString m_strCreator;
	CString m_strDescription;
public:
	virtual void ReadonlyPage(BOOL bReadonly);
	virtual HRESULT UpdateProp(BOOL bUpdate) ;
	CString m_strCreateDate;
};


