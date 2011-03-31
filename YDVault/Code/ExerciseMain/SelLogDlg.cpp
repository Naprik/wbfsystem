// SelLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "SelLogDlg.h"


// CSelLogDlg dialog

IMPLEMENT_DYNAMIC(CSelLogDlg, CDialog)

CSelLogDlg::CSelLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelLogDlg::IDD, pParent)
{

}

CSelLogDlg::~CSelLogDlg()
{
}

void CSelLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}


BEGIN_MESSAGE_MAP(CSelLogDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelLogDlg::OnBnClickedOk)
END_MESSAGE_MAP()

typedef CList<long,long> LongList;
typedef CList<CString,CString&> StringList;

int GetFileList(CString path,CString ext,LongList& sizelist,StringList& namelist)
{
	CString FileName;
	WIN32_FIND_DATA fdata = {0};  
	BOOL bFinished = FALSE;
	int nIndex = 0;

	FileName = path + "*." + ext;
	HANDLE hSearch = FindFirstFile(FileName.GetBuffer(0),&fdata);
	if(hSearch == INVALID_HANDLE_VALUE)    
		return 0;

	while(!bFinished)  
	{  
		if(!(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (nIndex==0)
			{
				sizelist.AddHead((long)fdata.nFileSizeLow);
				FileName.Format(L"%s",fdata.cFileName);
				namelist.AddHead(FileName);
			}
			else
			{
				POSITION pos;
				pos = sizelist.FindIndex(nIndex);
				sizelist.InsertAfter(pos,(long)fdata.nFileSizeLow);
				pos = namelist.FindIndex(nIndex);
				FileName.Format(L"%s",fdata.cFileName);
				namelist.InsertAfter(pos,FileName);
			}
			nIndex++;
		}

		if(!FindNextFile(hSearch, &fdata))    
		{  
			if (GetLastError() == ERROR_NO_MORE_FILES)    
				bFinished = TRUE;  
		}    
	}
	FindClose(hSearch);

	return nIndex;
}
// CSelLogDlg message handlers

BOOL CSelLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化试卷列表
	m_listLog.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_listLog.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_listLog.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_listLog.InsertColumn(1, L"练习名称", LVCFMT_LEFT,iWidth*5/6 - 2);

	WIN32_FIND_DATA fdata = {0};  
	BOOL bFinished = FALSE;
	int nIndex = 0;

	CString strFileName;
	CString strPath = g_LogPath;
	strPath += TEXT("\\");
	LongList lList;
	StringList strList;
	GetFileList(strPath, TEXT("xml"), lList, strList);
	
	POSITION pos = strList.GetHeadPosition();
	while (pos != NULL)
	{
		CString filename = strList.GetNext(pos);
		CString strIndex(_T(""));
		strIndex.Format(_T("%d"), m_listLog.GetItemCount()+1);
		int index = m_listLog.InsertItem(m_listLog.GetItemCount(), strIndex);
		m_listLog.SetItemText(index,1, filename);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelLogDlg::OnBnClickedOk()
{
	int nCount = m_listLog.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一练习！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_listLog.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_listLog.GetNextSelectedItem(pos);
	m_strFilename = m_listLog.GetItemText(nItem, 1);
	OnOK();
}
