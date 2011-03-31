// ShowListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShowListDlg.h"


// CShowListDlg dialog

IMPLEMENT_DYNAMIC(CShowListDlg, CDialog)

CShowListDlg::CShowListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowListDlg::IDD, pParent)
{

}

CShowListDlg::~CShowListDlg()
{
}

void CShowListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MARKS, m_UnitList);
}


BEGIN_MESSAGE_MAP(CShowListDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShowListDlg message handlers

BOOL CShowListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_UnitList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_UnitList.GetWindowRect(rect);

	ShowWindow(SW_MAXIMIZE);

	int iWidth = rect.Width();
	m_UnitList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/10);
	m_UnitList.InsertColumn(1, L"学号", LVCFMT_LEFT,iWidth*2/10 -20);
	m_UnitList.InsertColumn(2, L"姓名", LVCFMT_LEFT,iWidth/10);
	m_UnitList.InsertColumn(3, L"准考证号", LVCFMT_LEFT,iWidth*2/10);
	m_UnitList.InsertColumn(4, L"试卷名", LVCFMT_LEFT,iWidth*2/10);
	m_UnitList.InsertColumn(5, L"分数", LVCFMT_LEFT,iWidth*1/10 -2);
	m_UnitList.InsertColumn(6, L"考试时间", LVCFMT_LEFT,iWidth*2/10 +20);
	HRESULT hr = E_FAIL;
	m_UnitList.DeleteAllItems();
	for(std::list<CQueryMarkInfo*>::const_iterator itr = m_pLstQueryMarkInfo->begin();
		itr != m_pLstQueryMarkInfo->end();++itr)
	{
		CString strIndex(_T(""));
		strIndex.Format(_T("%d"), m_UnitList.GetItemCount()+1);
		int index = m_UnitList.InsertItem(m_UnitList.GetItemCount(), strIndex);
		m_UnitList.SetItemText(index,1, (*itr)->m_strStuID);
		m_UnitList.SetItemText(index,2, (*itr)->m_strStuName);
		m_UnitList.SetItemText(index,3, (*itr)->m_strExamID);
		CString strPaperName;
		hr = (*itr)->GetPaperName(strPaperName);
		if(FAILED(hr))
		{
			return FALSE;
		}
		m_UnitList.SetItemText(index,4, strPaperName);
		CString strMark;
		strMark.Format(_T("%.0f"),(*itr)->m_fMark);
		m_UnitList.SetItemText(index,5, strMark);
		m_UnitList.SetItemText(index,6, (*itr)->m_strExamTime);
		m_UnitList.SetItemData(index, (DWORD_PTR)(*itr));
	}
	m_UnitList.SetSpaceRowColor(TRUE);
	m_UnitList.Invalidate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShowListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;   //获取控件变化前的大小  
	GetWindowRect(&rect);

	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;
	rect.bottom -= 10;
	if(m_UnitList.GetSafeHwnd() != NULL)
	{
		m_UnitList.MoveWindow(rect);//设置控件大小
	}
}
