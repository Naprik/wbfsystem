#pragma once

#include "stdafx.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "../ObjRef/PropQueryContidition.h"
#include "resource.h"
#include "afxwin.h"
#include "BCGPComboBox.h"
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "afxcmn.h"

// CFormExamAddress form view

class BASE_DLL_API CFormExamAddress : public CYdFormView
{
	DECLARE_DYNCREATE(CFormExamAddress)

protected:
	CFormExamAddress();           // protected constructor used by dynamic creation
	virtual ~CFormExamAddress();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_FORM_EXAMADDR };
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
	virtual void OnInitialUpdate();

	afx_msg void OnBnClickedBtnDmExamaddrAdd();
	afx_msg void OnBnClickedBtnDmExamaddrEdit();
	afx_msg void OnBnClickedBtnDmExamaddrDel();
	afx_msg void OnBnClickedBtnDmExamaddrStuEdit();
	afx_msg void OnBnClickedBtnDmExamaddrSearch();
	afx_msg void OnBnClickedBtnDmQexamaddrBegin();
	afx_msg void OnBnClickedBtnDmQexamaddrPrev();
	afx_msg void OnBnClickedBtnDmQexamaddrNext();
	afx_msg void OnBnClickedBtnDmQexamaddrEnd();

private:
	HRESULT AddExamAddrItem(CYDObjectRef* _pref);
	HRESULT ExeQuery(BOOL _bFirstQuery = false);
	HRESULT InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion);

private:
	CBCGPSortListCtrl				m_ExamAddrListCtrl;
	std::list<CYDObjectRef*>	m_lstExamAddrCache;
	CBCGPKeyGridCtrl				m_QueryGrid;
	UINT m_utotalNumber;
	UINT m_uPageSize;
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*>	m_lstPropQuery;
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnBnClickedBtnDmExamaddrOutput();
};


