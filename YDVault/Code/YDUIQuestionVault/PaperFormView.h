#pragma once
#include "../YDFormUIBase/YdFormView.h"
#include "resource.h"
class BASE_DLL_API CPaperFormView :
	public CYdFormView
{
	DECLARE_DYNCREATE(CPaperFormView)

protected:
	CPaperFormView();           // protected constructor used by dynamic creation
	virtual ~CPaperFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_PAPER };
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

public:
	CString				m_strName;
	CString				m_strCreator;
	CString				m_strCreateDate;
	CString				m_strDescription;
};
