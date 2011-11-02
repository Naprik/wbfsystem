#pragma once
#include "afxwin.h"
#include "QuestionRecordStruct.h"

class CQuestionDlg :
	public CDialog
{
	DECLARE_DYNAMIC(CQuestionDlg)

	// Construction
public:
	CQuestionDlg();
	CQuestionDlg(UINT nIDTemplate, CWnd *pParent= NULL);
	CQuestionDlg(LPCTSTR lpszTemplateName, CWnd *pParentWnd= NULL);

	virtual HRESULT PersistData();

	CQuestionRecordStruct*		m_pRecord;
	BOOL m_bShowStdAnswer;
};
