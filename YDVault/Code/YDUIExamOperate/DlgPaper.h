#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "../YDExamObjRef\AnswerRecord.h"

// CDlgPaper dialog

class CDlgPaper : public CDialog
{
	DECLARE_DYNAMIC(CDlgPaper)

public:
	CDlgPaper(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPaper();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_PAPER };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	HRESULT InitSubject();
	HRESULT ValidateData(VARIANT_BOOL* _bValidate);
	HRESULT CreateAnswerRecord();
	HRESULT CreateAnswerRecordByBeginEnd(UINT uBegin,UINT uEnd,
										std::list<CAnswerRecord*>& _lstRecord);
	HRESULT CreatePaperInSubject(CYDObjectRef* _pPaper);//创建当前选择的科目与试卷的关系
	HRESULT CreateAnswer(CYDObjectRef* _pPaper);//创建试卷的标准答案和分数
	std::list<CYDObjectRef*>	m_lstClr;
	std::list<CAnswerRecord*>	m_lstAnswerRecord;
public:
	CComboBox m_cmbInSubject;
	virtual BOOL OnInitDialog();
	CString m_strPaperCode;
	CString m_strPaperName;
	UINT m_uBegin1;
	UINT m_uEnd1;
	UINT m_uCount1;
	UINT m_uBegin2;
	UINT m_uEnd2;
	UINT m_uCount2;
	UINT m_uBegin3;
	UINT m_uEnd3;
	UINT m_uCount3;
	CComboBox m_cmbMulti1;
	CComboBox m_cmbMulti2;
	CComboBox m_cmbMulti3;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonEditAnswer();
};
