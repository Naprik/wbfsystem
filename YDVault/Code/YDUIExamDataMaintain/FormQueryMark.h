#pragma once
#include "stdafx.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "resource.h"
#include "afxwin.h"
#include "BCGPComboBox.h"
#include "BCGPGridCtrl.h"
#include "afxcmn.h"
#include "../ObjRef/PropQueryContidition.h"
#include "QueryByClass.h"
#include "QueryByStudent.h"

// CFormStudent form view

class BASE_DLL_API CFormQueryMark : public CYdFormView
{
	DECLARE_DYNCREATE(CFormQueryMark)

protected:
	CFormQueryMark();           // protected constructor used by dynamic creation
	virtual ~CFormQueryMark();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_MARKQUERY };
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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

 	afx_msg void OnBnClickedBtnDmQMarkSearch();
 	afx_msg void OnBnClickedBtnDmQMarkBegin();
 	afx_msg void OnBnClickedBtnDmQMarkPrev();
 	afx_msg void OnBnClickedBtnDmQMarkNext();
 	afx_msg void OnBnClickedBtnDmQMarkEnd();

private:
	HRESULT AddItem(CYDObjectRef* _pref);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);
	HRESULT InsertlstToListCtrl(std::list<CYDObjectRef*> &lst);
	HRESULT AddPaperPropQueryContidion();
private:
	CBCGPComboBox				m_cmbPaper;
	CBCGPSortListCtrl			m_UnitList;

	std::list<CYDObjectRef*>	m_lstPaperCache;
	std::list<CYDObjectRef*>	m_lstUnitCache;
	UINT						m_utotalNumber;
	UINT						m_uPageSize;
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*>	m_lstPropQuery;
	std::list<OBJID>					m_lstSelClass;

	OBJID								m_paperID;
	BOOL								m_bFromStu;

	CTabCtrl					m_Tab;
	CQueryByStudent				m_PageStudent;
	CQueryByClass				m_PageClass;
public:
	afx_msg void OnTcnSelchangeDmQmarkTab(NMHDR *pNMHDR, LRESULT *pResult);
};


