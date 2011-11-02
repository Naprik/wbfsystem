#pragma once
#include "QuestionRecordStruct.h"
#include "QuestionAreaItem.h"
// CDlgQuestionArea dialog

class CDlgQuestionArea : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuestionArea)

public:
	CDlgQuestionArea(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgQuestionArea();

// Dialog Data
	enum { IDD = IDD_DLG_QUESTION_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();

	BOOL Reset();

	void CleanDlg();

	BOOL ShowQuestion(BOOL bShowAnswer);

	HRESULT GetStuAnswers(std::list<CString>* pListAns);
	
public:

	std::list<CQuestionAreaItem*>			m_lstDlgChoice;

	CRect								m_MainRect;
	CQuestionRecordStruct*				m_pRecord;
	int									m_nCurrentHeight;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
