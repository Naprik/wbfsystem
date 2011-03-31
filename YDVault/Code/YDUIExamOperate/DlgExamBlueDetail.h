#pragma once
#include "Resource.h"
#include "afxwin.h"

// CDlgExamBlueDetail dialog

class CDlgExamBlueDetail : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgExamBlueDetail)

public:
	CDlgExamBlueDetail(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExamBlueDetail();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_EXAM_BLUE_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	HRESULT ShowCurTime();
	BOOL ValidateRollCallData();//点名之前先校验数据
	BOOL ValiateFMChannel(CString strFmFrequency);//校验FM频率
	//_bUpdate= True将当前数据存到pGStruct中
	//_bUpdate=False,将pGStruct中数据存到当前的变量中
	HRESULT UpdateGStruct(BOOL _bUpdate);
	//由iChoiceAnswer得到在cmb中选择的Item
	int ChoiceAnswerToSel(int _iChoiceAnswer);
	//将选择的Item转化为ChoiceAnswer
	int SelToChoiceAnswer(int _iSel);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckListenOne();
	afx_msg void OnBnClickedCheckListenTwo();
	afx_msg void OnBnClickedCheckListenThree();
	BOOL m_bListenOne;
	BOOL m_bListenTwo;
	BOOL m_bListenThree;
	afx_msg void OnBnClickedCheckProgramTwo();
	afx_msg void OnBnClickedCheckProgramThree();
	afx_msg void OnBnClickedCheckProgramOne();
	BOOL m_bProgramTwo;
	BOOL m_bProgramThree;
	BOOL m_bProgramOne;
	afx_msg void OnBnClickedOk();
	COleDateTime m_timeListenOneStart;
	COleDateTime m_timeListenOneEnd;
	COleDateTime m_timeListenTwoStart;
	COleDateTime m_timeListenTwoEnd;
	COleDateTime m_timeListenThreeStart;
	COleDateTime m_timeListenThreeEnd;
	CString m_strFmFrequency;
	CString m_strStartNO1;
	CString m_strEndNO1;
	CComboBox m_cmbChooseAnswer1;
	CComboBox m_cmbChooseType1;
	CString m_strStartNO2;
	CString m_strEndNO2;
	CComboBox m_cmbChooseAnswer2;
	CComboBox m_cmbChooseType2;
	CString m_strStartNO3;
	CString m_strEndNO3;
	CComboBox m_cmbChooseAnswer3;
	CComboBox m_cmbChooseType3;
};
