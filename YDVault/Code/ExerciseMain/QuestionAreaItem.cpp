#include "StdAfx.h"
#include "QuestionAreaItem.h"


IMPLEMENT_DYNAMIC(CQuestionAreaItem, CDialog)


CQuestionAreaItem::CQuestionAreaItem()
{
}
//*************************************************************************************
CQuestionAreaItem::CQuestionAreaItem (UINT nIDTemplate, CWnd *pParent/*= NULL*/) : 
CDialog (nIDTemplate, pParent)
{
}
//*************************************************************************************
CQuestionAreaItem::CQuestionAreaItem (LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/) : 
CDialog(lpszTemplateName, pParentWnd)
{
}

HRESULT CQuestionAreaItem::GetAnswers(CString* pStrAnswer)
{
	return S_OK;
}