// StdAnswerDetialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "StdAnswerDetialDlg.h"
#include "afxdialogex.h"
#include "../ObjRef/YDQuestionRef.h"

namespace 
{
const int COL_QUESTION_NO = 0;		//题型那一列
const int COL_QUESTION_ANSWER = 1;	//难度
const int COL_QUESTION_DETIAL =2;
}

// CStdAnswerDetialDlg dialog

IMPLEMENT_DYNAMIC(CStdAnswerDetialDlg, CDialogEx)

CStdAnswerDetialDlg::CStdAnswerDetialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStdAnswerDetialDlg::IDD, pParent)
{

}

CStdAnswerDetialDlg::~CStdAnswerDetialDlg()
{
}

void CStdAnswerDetialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStdAnswerDetialDlg, CDialogEx)
END_MESSAGE_MAP()


// CStdAnswerDetialDlg message handlers


BOOL CStdAnswerDetialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rectBK;
	GetDlgItem(IDC_LIST_STD_ANSWER_DETIAL)->GetWindowRect(&rectBK);
	int iWidth = rectBK.Width();
	GetDlgItem(IDC_LIST_STD_ANSWER_DETIAL)->ShowWindow(SW_HIDE);
	ScreenToClient(&rectBK);
	if (!m_GridCtrlDetial.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_GridCtrlDetial.EnableDragHeaderItems(FALSE);
	m_GridCtrlDetial.EnableGroupByBox (FALSE);
	m_GridCtrlDetial.SetWholeRowSel(TRUE);
	m_GridCtrlDetial.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_GridCtrlDetial.SetColorTheme (colors);

	CString strNO = L"序号";
	m_GridCtrlDetial.InsertColumn(COL_QUESTION_NO, strNO, iWidth/12);

	CString strAnswer = L"标准答案";
	m_GridCtrlDetial.InsertColumn(COL_QUESTION_ANSWER, strAnswer, iWidth* 4/12);

	CString strDetial = L"答案解析";
	m_GridCtrlDetial.InsertColumn(COL_QUESTION_DETIAL, strDetial, iWidth* 7/12 -4);

	for (int iColumn = 0; iColumn < m_GridCtrlDetial.GetColumnCount(); iColumn++)
	{
		m_GridCtrlDetial.SetHeaderAlign(iColumn,HDF_CENTER);
		m_GridCtrlDetial.SetColumnAlign(iColumn, HDF_LEFT);
	}
	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CStdAnswerDetialDlg::FillList()
{
	std::list<std::pair<CString,CString>> lstStdAnswers;
	m_pQuestionRef->GetStandardAnswer(lstStdAnswers);
	int nCount = 1;
	for (auto itr=lstStdAnswers.begin(); itr!=lstStdAnswers.end(); ++itr, ++nCount)
	{
		CBCGPGridRow* pRow = m_GridCtrlDetial.CreateRow(m_GridCtrlDetial.GetColumnCount());

		pRow->GetItem(COL_QUESTION_NO)->SetValue(_variant_t(nCount));
		pRow->GetItem(COL_QUESTION_ANSWER)->SetValue(_variant_t(itr->first));
		pRow->GetItem(COL_QUESTION_DETIAL)->SetValue(_variant_t(itr->second));
		pRow->Enable(FALSE);
		m_GridCtrlDetial.AddRow(pRow);
	}

	return S_OK;
}