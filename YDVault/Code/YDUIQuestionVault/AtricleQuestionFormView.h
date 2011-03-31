#pragma once
#include "../YDFormUIBase/YdFormView.h"
#include "afxwin.h"


// CAtricleQuestionFormView form view

class BASE_DLL_API CAtricleQuestionFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CAtricleQuestionFormView)

protected:
	CAtricleQuestionFormView();           // protected constructor used by dynamic creation
	virtual ~CAtricleQuestionFormView(); 

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_ARTQICLEQUESTION };
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
	CString m_strArticle;
	int m_iHardLevel;
	CString m_strCreator;
	CString m_strCreateDate;
public:
	virtual void ReadonlyPage(BOOL bReadonly);
	virtual HRESULT UpdateProp(BOOL bUpdate) ;

	virtual void OnInitialUpdate();
	CComboBox m_cmbHardLevel;
};


