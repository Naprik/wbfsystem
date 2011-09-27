// DlgQuestion4.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgQuestion0.h"
#include "../ObjRef/YDChoiceQuestionRef.h"


// CDlgQuestion4 dialog

IMPLEMENT_DYNAMIC(CDlgQuestion0, CQuestionAreaItem)

CDlgQuestion0::CDlgQuestion0(CWnd* pParent /*=NULL*/)
	: CQuestionAreaItem(CDlgQuestion0::IDD, pParent)
{

}

CDlgQuestion0::~CDlgQuestion0()
{
}

void CDlgQuestion0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_ANSWER, m_strAnswer);
}


BEGIN_MESSAGE_MAP(CDlgQuestion0, CQuestionAreaItem)
END_MESSAGE_MAP()

HRESULT CDlgQuestion0::GetAnswers(CString* pStrAnswer)
{
	UpdateData(TRUE);
	*pStrAnswer = m_strAnswer;
	
	return S_OK;
}
// CDlgQuestion4 message handlers

BOOL CDlgQuestion0::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStatic* pText = (CStatic*)GetDlgItem(IDC_STATIC_NO);
	CString strNo;
	strNo.Format(TEXT("(%d.%d) "), m_mainIndex, m_index);
	pText->SetWindowText(strNo);
	CRect rectText;
	CRect rectMain;
	GetWindowRect(rectMain);
	GetDlgItem(IDC_RICHEDIT_ANSWER)->GetWindowRect(rectText);
	CRect rect(rectMain.left, rectMain.top, rectMain.right, rectText.bottom);
	MoveWindow(rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
