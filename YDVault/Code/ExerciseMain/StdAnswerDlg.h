#pragma once
#include "resource.h"
#include "../ObjRef/YDQuestionRef.h"
#include "QuestionAreaItem.h"
// CStdAnswerDlg dialog

class CStdAnswerDlg : public CQuestionAreaItem
{
	DECLARE_DYNAMIC(CStdAnswerDlg)

public:
	CStdAnswerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStdAnswerDlg();

// Dialog Data
	enum { IDD = IDD_DLG_QUESTION_STDANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strAnswer;
};
