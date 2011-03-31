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
#include <set>
#include "../ObjHelper/PaperAnsysHelper.h"
#include "afxdtctl.h"
// CFormPaper form view

class CPaperErrorResult;

class BASE_DLL_API CFormErrorReport : public CYdFormView
{
	DECLARE_DYNCREATE(CFormErrorReport)

protected:
	CFormErrorReport();           // protected constructor used by dynamic creation
	virtual ~CFormErrorReport();

public:
	enum { IDD = IDD_YDUIEXAM_DATA_OP_ERRORREPORT };
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


	afx_msg void OnBnClickedBtnDmStuDelqc();
	afx_msg void OnBnClickedBtnDmStuAddqc();
	afx_msg void OnBnClickedBtnDmStuClearqc();
	afx_msg void OnBnClickedBtnOpErrorreportSearch();

private:
	HRESULT CreateStuQuestionQueryProp();//在列表创建可以查询的学生条件名
	HRESULT CreatePaperPropQueryContidion();//根据combox创建试卷ID的查询条件
	HRESULT CreateStuPropQueryContidion(BOOL* pbFromStu);//根据列表上的选项创建学生的查询条件
	HRESULT ExeQuery(BOOL _bFirstQuery = FALSE);

	HRESULT InsertResToListCtrl();
private:
	CBCGPKeyGridCtrl						m_QueryGrid;
	std::list<CQueryProp*>				m_lstQueryProp;
	std::list<CYDObjectRef*>			m_lstSubjectCache;
	std::list<CYDObjectRef*>			m_lstPaperCache;
	CBCGPComboBox						m_cmbSubject;
	CBCGPSortListCtrl					m_ErrorReportList;
	std::list<CPropQueryContidition*>	m_lstStuPropQuery;//学生属性上的条件
	std::list<CPropQueryContidition*>   m_lstPaperPropQuery;//Paper试卷ID的条件

	BOOL								m_bFromStu;
	std::list<CPaperErrorResult*>		m_lstErrorResult;
	std::list<CYDObjectRef*>			m_lstMarks;
public:
	CComboBox m_cmbPaper;
	afx_msg void OnCbnSelchangeCmbOpErrorreportSubject();
	afx_msg void OnBnClickedBtnOpErrorreportOutput();
	afx_msg void OnNMDblclkListOpErrorreportList(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_begin;
	CDateTimeCtrl m_end;
//	afx_msg void OnLvnItemchangedListOpErrorreportList(NMHDR *pNMHDR, LRESULT *pResult);
};


