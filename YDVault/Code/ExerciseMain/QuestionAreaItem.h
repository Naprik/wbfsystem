#pragma once
#include "afxwin.h"
#include "QuestionRecordStruct.h"

class CQuestionAreaItem :
	public CDialog
{
	DECLARE_DYNAMIC(CQuestionAreaItem)

	// Construction
public:
	CQuestionAreaItem();
	CQuestionAreaItem(UINT nIDTemplate, CWnd *pParent= NULL);
	CQuestionAreaItem(LPCTSTR lpszTemplateName, CWnd *pParentWnd= NULL);

	virtual HRESULT GetAnswers(CString* pStrAnswer);

	std::list<CString>	m_lstChoices;

	CString				m_strAnswer;
	CYDQuestionRef*		m_pQuestion;
	int					m_index;
	int					m_mainIndex;
};
