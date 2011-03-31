#pragma once

#include <list>
#include "../ObjRef/YDQuestionRef.h"
#include "QuestionAreaItem.h"
// CDlgQuestion4 dialog

class CDlgQuestion1 : public CQuestionAreaItem
{
	DECLARE_DYNAMIC(CDlgQuestion1)

public:
	CDlgQuestion1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgQuestion1();

// Dialog Data
	enum { IDD = IDD_DLG_QUESTION_1 };

public:
	HRESULT GetAnswers(CString* pStrAnswer);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	
	virtual BOOL OnInitDialog();
};
