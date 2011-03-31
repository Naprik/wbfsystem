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
#include "mschart_paperansys.h"


class CYDStuMark;
class CYDPaper;
class CYDEAddrUnit;
// CFormStudent form view

class CQueryMarkInfo
{
public:
	CQueryMarkInfo();
	long			m_lPaperID;//试卷ID
	CString			m_strAnswer;//该学生的答案
	CString			m_strStuID;//当前学生学号
	CString			m_strStuName;//当前学生姓名
	CString			m_strExamID;//当前学生准考证号
	float			m_fMark;//分数
	CString			m_strExamTime;//考试时间
	HRESULT         GetPaper(CYDPaper*& pPaper);
	HRESULT			GetPaperName(CString &_strPaperName);
};

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
	HRESULT InsertlstToListCtrl();
	HRESULT ComputeAverageMark();//计算平均成绩
	HRESULT TransferMarkToQueryInfo(std::list<CYDObjectRef*> &_lstStuMark);
	HRESULT AddPaperPropQueryContidion();
private:
	CBCGPComboBox				m_cmbPaper;
	CBCGPSortListCtrl			m_UnitList;

	std::list<CYDObjectRef*>	m_lstPaperCache;
	std::list<CYDObjectRef*>	m_lstSubjectCache;
	std::list<CYDObjectRef*>	m_lstUnitCache;
	std::list<CQueryMarkInfo*>	m_lstQueryMarkInfo;
	std::list<CQueryMarkInfo*>	m_lstSelQueryMarkInfo;
	UINT						m_utotalNumber;
	UINT						m_uPageSize;
	UINT						m_uPage;//当前显示的页数,每页显示QUESTION_PAGE_COUNT条记录
	std::list<CPropQueryContidition*>	m_lstPropQuery;
	std::list<OBJID>					m_lstSelClass;

	OBJID								m_paperID;
	BOOL								m_bFromStu;
	std::list<CYDObjectRef*>	m_lstPaperConditon;//查询的试卷
	CTabCtrl					m_Tab;
	CQueryByStudent				m_PageStudent;
	CQueryByClass				m_PageClass;

	BOOL						m_bShowFromSearch;
	CDateTimeCtrl m_begin;
	CDateTimeCtrl m_end;
public:
	afx_msg void OnTcnSelchangeDmQmarkTab(NMHDR *pNMHDR, LRESULT *pResult);
	CMschart_paperansys m_chart;
	afx_msg void OnNMDblclkListDmQmarkMarks(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cmbSubject;
	afx_msg void OnCbnSelchangeCmbDmQmarkSubject();
	afx_msg void OnNMRClickListDmQmarkMarks(NMHDR *pNMHDR, LRESULT *pResult);
	//对一个学生成绩进行成绩分析查询，显示在图中
	afx_msg void OnMIStuMarkAna();
private:
	//在列表上查找，得到所有相同学生的stumark
	HRESULT GetSameQueryMarkInfoByStuID(CString stuID,std::list<CQueryMarkInfo*> *_lstQueryMarkInfo);
	//根据_lstStuMark画图
	//图的横坐标为试卷名，纵坐标为分数
	HRESULT DrawStuMarkByPaper(std::list<CQueryMarkInfo*> &_lstQueryMarkInfo);
	//显示查询结果分数段人次的图表
	//图的横坐标是分数段，纵坐标是在这个分数段的人次
	HRESULT DrawStuMarkCountSub(CMschart_paperansys& chart);
public:
	afx_msg void OnBnClickedBtnDmQmarkOutput();
	//特殊处理，用来显示当前全局变量中的试卷信息
	virtual HRESULT ExeSpecial();
private:
	//从全局变量中获取mac地址为_strMac的Unit
	HRESULT GetUnitByMac(CString _strMac,CYDEAddrUnit* &_pUnit );
public:
	CString m_strAverageMark;
	afx_msg void OnBnClickedBtnShowchart();
	afx_msg void OnBnClickedBtnShowlist();
};


