// DlgQuestion4.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgQuestion1.h"
#include "../ObjRef/YDChoiceQuestionRef.h"


// CDlgQuestion4 dialog

IMPLEMENT_DYNAMIC(CDlgQuestion1, CQuestionAreaItem)

CDlgQuestion1::CDlgQuestion1(CWnd* pParent /*=NULL*/)
	: CQuestionAreaItem(CDlgQuestion1::IDD, pParent)
{

}

CDlgQuestion1::~CDlgQuestion1()
{
}

void CDlgQuestion1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_ANSWER, m_strAnswer);
}


BEGIN_MESSAGE_MAP(CDlgQuestion1, CQuestionAreaItem)
END_MESSAGE_MAP()

HRESULT CDlgQuestion1::GetAnswers(CString* pStrAnswer)
{
	UpdateData(TRUE);
	*pStrAnswer = m_strAnswer.Trim();
	
	return S_OK;
}
// CDlgQuestion4 message handlers

BOOL CDlgQuestion1::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStatic* pText = (CStatic*)GetDlgItem(IDC_STATIC_TEXT);
	CString str;
	m_pQuestion->GetPropVal(TEXT("title"), str);
	CString strTilte;
	strTilte.Format(TEXT("(%d.%d) %s"),  m_mainIndex, m_index, str);
	pText->SetWindowText(strTilte);
	CRect rectText;
	CRect rectMain;
	GetWindowRect(rectMain);
	GetDlgItem(IDC_RICHEDIT_ANSWER)->GetWindowRect(rectText);
	CRect rect(rectMain.left, rectMain.top, rectMain.right, rectText.bottom);
	MoveWindow(rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
