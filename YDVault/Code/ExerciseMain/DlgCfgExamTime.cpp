// DlgCfgExamTime.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgCfgExamTime.h"
#include "ExamSelectQuestionCfgMgr.h"


// CDlgCfgExamTime dialog

IMPLEMENT_DYNAMIC(CDlgCfgExamTime, CDialog)

CDlgCfgExamTime::CDlgCfgExamTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgExamTime::IDD, pParent)
	, m_uTimeSpan(0)
{

}

CDlgCfgExamTime::~CDlgCfgExamTime()
{
}

void CDlgCfgExamTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_uTimeSpan);
	DDV_MinMaxUInt(pDX, m_uTimeSpan, 1, 500);
}


BEGIN_MESSAGE_MAP(CDlgCfgExamTime, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCfgExamTime::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCfgExamTime message handlers

void CDlgCfgExamTime::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CExamSelectQuestionCfgMgr::Instance()->m_uTimeSpan = m_uTimeSpan;
	HRESULT hr = E_FAIL;
	hr = CExamSelectQuestionCfgMgr::Instance()->Save();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	OnOK();
}

BOOL CDlgCfgExamTime::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_uTimeSpan = CExamSelectQuestionCfgMgr::Instance()->m_uTimeSpan;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
