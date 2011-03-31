#pragma once

#include "stdafx.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "resource.h"
#include "afxwin.h"
#include "BCGPComboBox.h"
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "afxcmn.h"
#include "../ObjRef/PropQueryContidition.h"
// CFormPaper form view

class BASE_DLL_API CFormPaper : public CYdFormView
{
	DECLARE_DYNCREATE(CFormPaper)

protected:
	CFormPaper();           // protected constructor used by dynamic creation
	virtual ~CFormPaper();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_FORM_PAPER };
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
	afx_msg void OnBnClickedBtnDmPaperAdd();
	afx_msg void OnBnClickedBtnDmPaperEdit();
	afx_msg void OnBnClickedBtnDmPaperDel();
	afx_msg void OnBnClickedBtnDmPaperAnswerEdit();
	afx_msg void OnBnClickedBtnDmPaperSearch();
	virtual void OnInitialUpdate();

private:
	HRESULT AddPaperItem(CYDObjectRef* _pref);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);

	HRESULT InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion);

private:
	CBCGPSortListCtrl			m_PaperListCtrl;
	CBCGPKeyGridCtrl				m_QueryGrid;
	std::list<CYDObjectRef*>	m_lstPaperCache;
	UINT m_utotalNumber;
	UINT m_uPageSize;
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*>	m_lstPropQuery;
public:
	afx_msg void OnBnClickedBtnDmQpaperBegin();
	afx_msg void OnBnClickedBtnDmQpaperPrev();
	afx_msg void OnBnClickedBtnDmQpaperNext();
	afx_msg void OnBnClickedBtnDmQpaperEnd();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnBnClickedBtnDmPaperOutput();
};


