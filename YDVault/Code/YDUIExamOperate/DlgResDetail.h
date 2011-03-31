#if !defined(AFX_DLGRESDETAIL_H__A6BA5197_0C28_459A_B554_1B22D0EBE9F3__INCLUDED_)
#define AFX_DLGRESDETAIL_H__A6BA5197_0C28_459A_B554_1B22D0EBE9F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"

#include "../UIBase/CustomdrawList.h"
#include "../YDExamObjRef/YDStuMark.h"
#include "../UIBase/BCGPSortListCtrl.h"

class CYDEAddrUnit;
class CQueryMarkInfo;


// DlgResDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgResDetail dialog
enum RESULT_TYPE //��ʾ�𰸵�����
{
	FROM_DB,//��ѯ���ݿ�
	FROM_TEACHER_EXAM,//�ӽ�ʦ������
	FROM_INRARED_EXAM //�����ʦ������
};


class CDlgResDetail : public CDialog
{
// Construction
public:
	CDlgResDetail(CWnd* pParent = NULL);   // standard constructor
	


// Dialog Data
	//{{AFX_DATA(CDlgResDetail)
	enum { IDD = IDD_YDUIEXAM_DIG_RES_DETAIL };
	CBCGPSortListCtrl	m_reslist;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgResDetail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgResDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkReslist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_stuNo;
	CString m_stuName;
	CString m_strSubject;
	CString m_strPaper;
	CString m_strRightItemNo;
	CString m_strErrorItemNo;
	CString m_strTotalScore;
	CString m_strScore;
	CYDEAddrUnit* m_pUnit;
	CStudev*	  m_pStudev;
	CYDPaper*	  m_pPaper;//�ں��⿼��ʱ��Ч
	CYDSubjectRef* m_pSubject;//�ں��⿼��ʱ��Ч
	CString		   m_strAnswer;//�ں��⿼��ʱ��Ч

	CQueryMarkInfo*	m_pQueryMarkInfo;

private:
	HRESULT InitAnswerInfo(CYDPaper* pPaper);
	HRESULT InsertAnswerByBeginEnd(long lBegin,long lEnd,
								std::list<CYDObjectRef*>& _lstStdAnswer,
								std::map<int, CString> &_mapAnswer,
								UINT &uErrorNum,
								UINT &uRightNum,
								double &dbMark );

public:
	RESULT_TYPE	m_ResultType;
	afx_msg void OnBnClickedResdetailOutput();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESDETAIL_H__A6BA5197_0C28_459A_B554_1B22D0EBE9F3__INCLUDED_)
