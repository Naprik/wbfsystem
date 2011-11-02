#include "StdAfx.h"
#include "ListBoxEx.h"
#include "../Base/FilePathHelper.h"

extern AFX_EXTENSION_MODULE UIBaseDLL;
CListBoxEx::CListBoxEx(void)
{
	CLogHelper helper;
	COleDateTime dtCur = COleDateTime::GetCurrentTime();
	CString strCur;
	strCur.Format(_T("%d-%d-%d"),dtCur.GetYear(),dtCur.GetMonth(),dtCur.GetDay());
	CString strLogPath = _T("\\log\\YD");
	strLogPath += strCur;
	strLogPath += _T(".log");
	CString LogMain;
	CFilePathHelper::GetMainCfgPath(LogMain);
	helper.SetLogPath(&m_log,LogMain,strLogPath);
}

CListBoxEx::~CListBoxEx(void)
{
}

int CListBoxEx::AddString(LPCTSTR lpszItem)
{
	int index = CListBox::AddString(lpszItem);
	SetCaretIndex(index);
	//m_log.Print(SERVER_DEBUG,PLMLOG(_T("CBomExcel::GetReportListItemCol failed at  pCxMetaReportListItem->get_index\n")));
	CString strLog;
	strLog.Format(_T("%s\n"),lpszItem);
	m_log.Print(SERVER_DEBUG,strLog);
	return index;
}
