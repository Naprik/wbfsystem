#include "StdAfx.h"
#include "ListBoxEx.h"


extern AFX_EXTENSION_MODULE UIBaseDLL;
CListBoxEx::CListBoxEx(void)
{
	TCHAR filename[MAX_PATH] = {0};
	DWORD nret = GetModuleFileName(NULL,filename,MAX_PATH);
	filename[nret] = 0;
	TCHAR* p = _tcsrchr(filename,_T('\\')); //bin/
	if(p!=NULL) *p = 0;
	CComBSTR MoudlePath = filename;
	CLogHelper helper;
	COleDateTime dtCur = COleDateTime::GetCurrentTime();
	CString strCur;
	strCur.Format(_T("%d-%d-%d"),dtCur.GetYear(),dtCur.GetMonth(),dtCur.GetDay());
	CString strLogPath = _T("\\log\\YD");
	strLogPath += strCur;
	strLogPath += _T(".log");
	helper.SetLogPath(&m_log,MoudlePath,strLogPath);
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
