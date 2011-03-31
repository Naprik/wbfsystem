#pragma once

#include <list>
#include "../ObjRef/YDQuestionRef.h"
#include "QuestionAreaItem.h"
// CDlgQuestion4 dialog

class CDlgQuestion4 : public CQuestionAreaItem
{
	DECLARE_DYNAMIC(CDlgQuestion4)

public:
	CDlgQuestion4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgQuestion4();

// Dialog Data
	enum { IDD = IDD_DLG_QUESTION_4 };

public:
	HRESULT GetAnswers(CString* pStrAnswer);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	
	virtual BOOL OnInitDialog();
};
