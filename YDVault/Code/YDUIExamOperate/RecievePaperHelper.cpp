#include "StdAfx.h"
#include "RecievePaperHelper.h"
#include "../ydcom\StaticYdComApp.h"

CRecievePaperHelper::CRecievePaperHelper(void)
{
}

CRecievePaperHelper::~CRecievePaperHelper(void)
{
}

HRESULT CRecievePaperHelper::PrepareRecievePaper()
{
	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	ASSERT(pGExamStruct);
// 	if(pGExamStruct->m_pExamDetail->bIsRollDone)
// 	{
// 		return S_OK;
// 	}
// 	else
// 	{
// 		AfxMessageBox(_T("学生没有开考，无法进入收卷模式！"));
// 		return S_FALSE;
// 	}
	return S_FALSE;
}
