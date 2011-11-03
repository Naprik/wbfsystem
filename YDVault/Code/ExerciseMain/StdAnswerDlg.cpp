// StdAnswerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "StdAnswerDlg.h"
#include "afxdialogex.h"


// CStdAnswerDlg dialog

IMPLEMENT_DYNAMIC(CStdAnswerDlg, CQuestionAreaItem)

CStdAnswerDlg::CStdAnswerDlg(CWnd* pParent /*=NULL*/)
	: CQuestionAreaItem(CStdAnswerDlg::IDD, pParent)
	, m_strAnswer(_T(""))
{

}

CStdAnswerDlg::~CStdAnswerDlg()
{
}

void CStdAnswerDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionAreaItem::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STDANSWER, m_strAnswer);
}


BEGIN_MESSAGE_MAP(CStdAnswerDlg, CQuestionAreaItem)
END_MESSAGE_MAP()


// CStdAnswerDlg message handlers


BOOL CStdAnswerDlg::OnInitDialog()
{
	CQuestionAreaItem::OnInitDialog();

	std::list<std::pair<CString,CString>> lstStdAnswers;
	m_pQuestion->GetStandardAnswer(lstStdAnswers);
	if (lstStdAnswers.size() > 0)
	{
		int index = 1;
		CString strFormat = L"(%d.%d) 答案：%s  答案解析：%s\r\n";
		CString strAnswer;
		for (auto itr=lstStdAnswers.begin(); itr!=lstStdAnswers.end(); ++itr, ++index)
		{
			strAnswer.Format(strFormat,  m_mainIndex, index, itr->first, itr->second);
			m_strAnswer += strAnswer;
		}
	}
	else
	{
		m_strAnswer = L"题库没有录入参考答案！";
	}
	
	UpdateData(FALSE);
	return TRUE;;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
