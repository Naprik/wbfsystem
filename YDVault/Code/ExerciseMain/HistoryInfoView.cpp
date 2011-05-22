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
	
	DDX_Control(pDX, IDC_LIST_EXERCISE, m_ExerciseListCtrl);
	DDX_Control(pDX, IDC_LIST_EXAM, m_ExamListCtrl);
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


BOOL CHistoryInfoView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CHistoryInfoView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	m_ExerciseListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_ExerciseListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_ExerciseListCtrl.InsertColumn(0, L"ÐòºÅ", LVCFMT_CENTER, iWidth/8);
	m_ExerciseListCtrl.InsertColumn(1, L"Ãû³Æ", LVCFMT_CENTER,iWidth*7/8 - 2);
	
	m_ExamListCtrl.InsertColumn(0, L"ÐòºÅ", LVCFMT_CENTER, iWidth/8);
	m_ExamListCtrl.InsertColumn(1, L"Ãû³Æ", LVCFMT_CENTER,iWidth*7/8 - 2);
}
