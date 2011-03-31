#pragma once

#include <list>
#include "../ObjRef/YDQuestionRef.h"
#include "QuestionAreaItem.h"
// CDlgQuestion4 dialog

class CDlgQuestion0 : public CQuestionAreaItem
{
	DECLARE_DYNAMIC(CDlgQuestion0)

public:
	CDlgQuestion0(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgQuestion0();

// Dialog Data
	enum { IDD = IDD_DLG_QUESTION_0 };

public:
	HRESULT GetAnswers(CString* pStrAnswer);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	
	virtual BOOL OnInitDialog();
};
