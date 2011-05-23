// HistoryInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "HistoryInfoView.h"
#include "ExerciseMainDlg.h"

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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EXAM, &CHistoryInfoView::OnNMDblclkListExam)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EXERCISE, &CHistoryInfoView::OnNMDblclkListExercise)
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

	m_ExerciseListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect rect;
	m_ExerciseListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_ExerciseListCtrl.InsertColumn(0, L"ÐòºÅ", LVCFMT_LEFT, iWidth/8);
	m_ExerciseListCtrl.InsertColumn(1, L"Ãû³Æ", LVCFMT_LEFT,iWidth*7/8 - 2);
	InsertExerciseLog();
	m_ExamListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ExamListCtrl.InsertColumn(0, L"ÐòºÅ", LVCFMT_LEFT, iWidth/8);
	m_ExamListCtrl.InsertColumn(1, L"Ãû³Æ", LVCFMT_LEFT,iWidth*7/8 - 2);
	InsertExamLog();
}

bool CHistoryInfoView::InsertExerciseLog()
{
	CString logindex(_T(""));
	CFileFind find;
	CString logpath = g_LogPath;
	logpath += TEXT("\\exercise\\*.xml");
	BOOL hasfile = find.FindFile(logpath);
	
	while (hasfile)
	{
		hasfile = find.FindNextFile();
		CString logname = find.GetFileTitle();
		logindex.Format(_T("%d"), m_ExerciseListCtrl.GetItemCount()+1);
		int index = m_ExerciseListCtrl.InsertItem(m_ExerciseListCtrl.GetItemCount(), logindex);
		m_ExerciseListCtrl.SetItemText(index,1, logname);
			
	}

	return true;
}

bool CHistoryInfoView::InsertExamLog()
{
	CString logindex(_T(""));
	CFileFind find;
	CString logpath = g_LogPath;
	logpath += TEXT("\\exam\\*.xml");
	BOOL hasfile = find.FindFile(logpath);
	
	while (hasfile)
	{
		hasfile = find.FindNextFile();
		CString logname = find.GetFileTitle();
		logindex.Format(_T("%d"), m_ExamListCtrl.GetItemCount()+1);
		int index = m_ExamListCtrl.InsertItem(m_ExamListCtrl.GetItemCount(), logindex);
		m_ExamListCtrl.SetItemText(index,1, logname);
			
	}
	return true;
}

void CHistoryInfoView::OnNMDblclkListExam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CHistoryInfoView::OnNMDblclkListExercise(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	HRESULT hr = E_FAIL;
	CString logname = m_ExerciseListCtrl.GetItemText(pNMListView->iItem, 1);
	
	logname += TEXT(".xml");
	CExerciseMainDlg dlg(this);
	dlg.m_bLoadMode = TRUE;
	dlg.m_strLogName = logname;
	if (dlg.DoModal() == IDOK)
	{
	}
	*pResult = 0;
}
