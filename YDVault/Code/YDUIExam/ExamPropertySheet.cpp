#include "StdAfx.h"
#include "ExamPropertySheet.h"
#include "../Base/AutoClean.h"

CExamPropertySheet::CExamPropertySheet(void)
{
}

CExamPropertySheet::~CExamPropertySheet(void)
{
	for(std::map<CString,CFormView*>::const_iterator itr = m_mapView.begin();
		itr != m_mapView.end();++itr)
	{
		CFormView* pFormView = (*itr).second;
		CPtrAutoClean<CFormView> clr(pFormView);
	}
	
}

HRESULT CExamPropertySheet::ShowExam(CString _strText)
{
	HRESULT hr = E_FAIL;
	ASSERT(!_strText.IsEmpty());
	std::map<CString,CFormView*>::iterator itr = m_mapView.find(_strText);
	if (itr != m_mapView.end())
	{
		return S_OK;
	}
	if(_strText.CompareNoCase(_T("院系信息")) == 0)
	{

	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}
