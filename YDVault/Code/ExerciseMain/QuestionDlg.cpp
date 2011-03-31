#include "StdAfx.h"
#include "QuestionDlg.h"


IMPLEMENT_DYNAMIC(CQuestionDlg, CDialog)


CQuestionDlg::CQuestionDlg()
{
}
//*************************************************************************************
CQuestionDlg::CQuestionDlg (UINT nIDTemplate, CWnd *pParent/*= NULL*/) : 
CDialog (nIDTemplate, pParent)
{
}
//*************************************************************************************
CQuestionDlg::CQuestionDlg (LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/) : 
CDialog(lpszTemplateName, pParentWnd)
{
}
HRESULT CQuestionDlg::PersistData()
{
	return S_OK;
}