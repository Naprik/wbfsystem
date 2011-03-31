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
#include <set>
#include "../ObjHelper/PaperAnsysHelper.h"
#include "mschart_paperansys.h"
// CFormPaper form view

class BASE_DLL_API CFormPaperAnsys : public CYdFormView
{
	DECLARE_DYNCREATE(CFormPaperAnsys)

protected:
	CFormPaperAnsys();           // protected constructor used by dynamic creation
	virtual ~CFormPaperAnsys();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_OP_PAPERANSYS };
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

private:
	HRESULT AddPaperMarkItem(CYDObjectRef* _pref);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);

	HRESULT InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion);
	HRESULT AddClassItem(CYDObjectRef* pObjRef);
	HRESULT AddSelClassItem(CYDObjectRef* pObjRef);
	BOOL IsClassExist(CYDObjectRef* pStu);
	HRESULT RemoveSelClass(CYDObjectRef* pStu);
	HRESULT AddPaperPropQueryContidion();
private:
	CBCGPSortListCtrl			m_MarkListCtrl;

	CBCGPSortListCtrl			m_AllClassList;
	CBCGPSortListCtrl			m_SelClassList;
	std::list<CYDObjectRef*>	m_lstMarkCache;
	std::list<CYDObjectRef*>	m_lstPaperCache;
	std::list<CYDObjectRef*>	m_lstDeptCache;
	std::list<CYDObjectRef*>	m_lstClsCache;
	std::list<CYDObjectRef*>	m_lstSelClsCache;
	std::list<CPaperAnsysHelperResult*>	m_lstPaperAnsysResCache;
	
	std::list<CPropQueryContidition*>	m_lstPropQuery;
	std::map<UINT, CPaperAnsysHelperResult*> m_mapRes;
	int										 m_Total;
	CComboBox m_cmbPaper;
	CComboBox m_cmbDept;
	std::set<OBJID>				m_setExistCls;
	CMschart_paperansys m_chart;
public:
	afx_msg void OnBnClickedBtnOPQpaperMarkSearch();

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnCbnSelchangeCmbOpPaperansysDept();
	afx_msg void OnBnClickedBtnOpPaperansysAdd();
	afx_msg void OnBnClickedBtnOpPaperansysDel();

	HRESULT InsertList(UINT iTotal, std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
	afx_msg void OnNMDblclkListOpPaperansysMark(NMHDR *pNMHDR, LRESULT *pResult);

	HRESULT ShowPicture(UINT iTotal, std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
	
	afx_msg void OnBnClickedBtnOpPaperansysOutput();
	virtual HRESULT ExeSpecial();
	afx_msg void OnBnClickedBtnShowchart();
	afx_msg void OnBnClickedBtnShowlist();
};


