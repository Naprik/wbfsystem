// HistoryInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "HistoryInfoView.h"


// CHistoryInfoView

IMPLEMENT_DYNCREATE(CHistoryInfoView, CYdFormView)

CHistoryInfoView::CHistoryInfoView()
	: CYdFormView(CHistoryInfoView::IDD)
{

}

CHistoryInfoView::~CHistoryInfoView()
{
}

void CHistoryInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHistoryInfoView, CFormView)
END_MESSAGE_MAP()


// CHistoryInfoView diagnostics

#ifdef _DEBUG
void CHistoryInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHistoryInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHistoryInfoView message handlers
