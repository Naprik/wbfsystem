// QuestionViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../ObjRef/YDQuestionRef.h"
#include "QuestionViewDlg.h"
#include "../Base\AutoClean.h"
#include "../Include\ShowErr.h"
#include "resource.h"
#include "../UIBase\ButtonItem.h"


// CQuestionViewDlg dialog
const int cColSerialNo = 0;
const int cColFileName = 1;
const int cColPlay = 2;

#define IDC_PLAY_MEDIA WM_USER+10000

IMPLEMENT_DYNAMIC(CQuestionViewDlg, CDialog)

CQuestionViewDlg::CQuestionViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestionViewDlg::IDD, pParent)
	, m_strQuestion(_T(""))
{
	m_pQuestionRef = NULL;
}

CQuestionViewDlg::~CQuestionViewDlg()
{
	CListAutoClean<CYDMediaRef> clr(m_lstMedia);
}

void CQuestionViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_QUESTION, m_strQuestion);
}


BEGIN_MESSAGE_MAP(CQuestionViewDlg, CDialog)
	ON_COMMAND(IDC_PLAY_MEDIA, OnPlayMedia)
END_MESSAGE_MAP()


// CQuestionViewDlg message handlers

BOOL CQuestionViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rectBK;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rectBK);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	if (!m_Grid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_Grid.EnableDragHeaderItems(FALSE);
	//m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	m_Grid.InsertColumn(cColSerialNo, L"序号", 100);
	m_Grid.InsertColumn(cColFileName, L"多媒体", 280);
	m_Grid.InsertColumn(cColPlay, L"播放", 100);
	ASSERT(m_pQuestionRef);
	HRESULT hr = E_FAIL;
	hr = m_pQuestionRef->GetAllMedia(&m_lstMedia);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	for(std::list<CYDMediaRef*>::const_iterator itr = m_lstMedia.begin();
		itr != m_lstMedia.end();++itr)
	{
		CString strMediaLabel;
		hr = (*itr)->GetLabel(&strMediaLabel);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		int iCount = m_Grid.GetRowCount();
		CString strIndex;
		strIndex.Format(_T("%d"),iCount+1);
		CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		ASSERT(pRow);
		pRow->GetItem(cColSerialNo)->SetValue(CComVariant(strIndex));
		pRow->GetItem(cColFileName)->SetValue(CComVariant(strMediaLabel));
		pRow->ReplaceItem (cColPlay, new CButtonItem (_T("播放"), IDC_PLAY_MEDIA,this));
		pRow->SetData(DWORD_PTR(*itr));
		m_Grid.AddRow(pRow);
	}
	m_pQuestionRef->ToString(&m_strQuestion);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR CQuestionViewDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}

void CQuestionViewDlg::OnPlayMedia()
{
	AfxMessageBox(_T("Play"));
}