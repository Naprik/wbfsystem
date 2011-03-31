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
	long			m_lPaperID;//�Ծ�ID
	CString			m_strAnswer;//��ѧ���Ĵ�
	CString			m_strStuID;//��ǰѧ��ѧ��
	CString			m_strStuName;//��ǰѧ������
	CString			m_strExamID;//��ǰѧ��׼��֤��
	float			m_fMark;//����
	CString			m_strExamTime;//����ʱ��
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
	HRESULT ComputeAverageMark();//����ƽ���ɼ�
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
	UINT						m_uPage;//��ǰ��ʾ��ҳ��,ÿҳ��ʾQUESTION_PAGE_COUNT����¼
	std::list<CPropQueryContidition*>	m_lstPropQuery;
	std::list<OBJID>					m_lstSelClass;

	OBJID								m_paperID;
	BOOL								m_bFromStu;
	std::list<CYDObjectRef*>	m_lstPaperConditon;//��ѯ���Ծ�
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
	//��һ��ѧ���ɼ����гɼ�������ѯ����ʾ��ͼ��
	afx_msg void OnMIStuMarkAna();
private:
	//���б��ϲ��ң��õ�������ͬѧ����stumark
	HRESULT GetSameQueryMarkInfoByStuID(CString stuID,std::list<CQueryMarkInfo*> *_lstQueryMarkInfo);
	//����_lstStuMark��ͼ
	//ͼ�ĺ�����Ϊ�Ծ�����������Ϊ����
	HRESULT DrawStuMarkByPaper(std::list<CQueryMarkInfo*> &_lstQueryMarkInfo);
	//��ʾ��ѯ����������˴ε�ͼ��
	//ͼ�ĺ������Ƿ����Σ�������������������ε��˴�
	HRESULT DrawStuMarkCountSub(CMschart_paperansys& chart);
public:
	afx_msg void OnBnClickedBtnDmQmarkOutput();
	//���⴦��������ʾ��ǰȫ�ֱ����е��Ծ���Ϣ
	virtual HRESULT ExeSpecial();
private:
	//��ȫ�ֱ����л�ȡmac��ַΪ_strMac��Unit
	HRESULT GetUnitByMac(CString _strMac,CYDEAddrUnit* &_pUnit );
public:
	CString m_strAverageMark;
	afx_msg void OnBnClickedBtnShowchart();
	afx_msg void OnBnClickedBtnShowlist();
};


