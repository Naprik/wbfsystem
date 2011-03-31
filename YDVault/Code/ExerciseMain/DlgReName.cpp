// DlgReName.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgReName.h"


// CDlgReName dialog

IMPLEMENT_DYNAMIC(CDlgReName, CDialog)

CDlgReName::CDlgReName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReName::IDD, pParent)
	, m_strName(_T(""))
{

}

CDlgReName::~CDlgReName()
{
}

void CDlgReName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgReName, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgReName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgReName message handlers

void CDlgReName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strName.IsEmpty())
	{
		AfxMessageBox(_T("名字不能为空！"));
		return;
	}

	OnOK();
}

BOOL CDlgReName::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
