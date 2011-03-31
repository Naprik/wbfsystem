#pragma once
#include "stdafx.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "resource.h"
#include "afxwin.h"
#include "BCGPComboBox.h"
#include "BCGPGridCtrl.h"
#include "afxcmn.h"


// CFormSubject form view

class BASE_DLL_API CFormSubject : public CYdFormView
{
	DECLARE_DYNCREATE(CFormSubject)

protected:
	CFormSubject();           // protected constructor used by dynamic creation
	virtual ~CFormSubject();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_FORM_SUBJECT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDmSubjNew();
	afx_msg void OnBnClickedDmSubjEdit();
	afx_msg void OnBnClickedDmSubjDel();

public:
	virtual void OnInitialUpdate();

private:
	HRESULT AddSubjectItem(CYDObjectRef* _pref);

private:
	CBCGPSortListCtrl			m_SubjectListCtrl;
	std::list<CYDObjectRef*>	m_lstSubjectCache;
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnBnClickedDmSubjEditpaper();
	afx_msg void OnBnClickedDmSubjOutput();
};


