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
// CFormStudent form view

class BASE_DLL_API CFormStudent : public CYdFormView
{
	DECLARE_DYNCREATE(CFormStudent)

protected:
	CFormStudent();           // protected constructor used by dynamic creation
	virtual ~CFormStudent();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_STUDENT };
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

	afx_msg void OnBnClickedBtnDmStuSearch();
	afx_msg void OnBnClickedBtnDmStuAdd();
	afx_msg void OnBnClickedBtnDmStuEdit();
	afx_msg void OnBnClickedBtnDmStuDel();
	afx_msg void OnCbnSelchangeCmbDmStuconnDept();
	afx_msg void OnBnClickedBtnDmQstuBegin();
	afx_msg void OnBnClickedBtnDmQstuPrev();
	afx_msg void OnBnClickedBtnDmQstuNext();
	afx_msg void OnBnClickedBtnDmQstuEnd();

private:
	void ClearClassCache();
	HRESULT AddItem(CYDObjectRef* _pref);
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);
	HRESULT InsertlstToListCtrl(std::list<CYDObjectRef*> &lst);
	HRESULT CreateQuestionQueryProp();
	HRESULT CreatePropQueryContidion();
private:
	CBCGPComboBox				m_cmbDept;
	CBCGPComboBox				m_cmbCls;
	CBCGPSortListCtrl			m_StuList;
	CBCGPKeyGridCtrl				m_gridConn;

	std::list<CYDObjectRef*>	m_lstDeptCache;
	std::list<CYDObjectRef*>	m_lstClsCache;
	std::list<CYDObjectRef*>	m_lstStudentCache;

	UINT						m_utotalNumber;
	UINT						m_uPageSize;
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*>	m_lstPropQuery;

	std::list<CQueryProp*>      m_lstQueryProp;//当前类型试题可以查询的属性
public:
	afx_msg void OnBnClickedBtnDmStuAddqc();
	afx_msg void OnBnClickedBtnDmStuDelqc();
	afx_msg void OnBnClickedBtnDmStuClearqc();
	afx_msg void OnBnClickedBtnDmStuOutput();
	afx_msg void OnBnClickedButtonInput();
};


