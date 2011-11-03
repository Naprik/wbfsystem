// DlgSelInputType.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSelInputType.h"
#include "afxdialogex.h"


// CDlgSelInputType dialog

IMPLEMENT_DYNAMIC(CDlgSelInputType, CDialogEx)

CDlgSelInputType::CDlgSelInputType(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSelInputType::IDD, pParent)
{
	m_iSelType = 0;
}

CDlgSelInputType::~CDlgSelInputType()
{
}

void CDlgSelInputType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSelInputType, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSelInputType::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSelInputType message handlers


void CDlgSelInputType::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CButton::GetCheck()
	if(((CButton*)GetDlgItem(IDC_RADIO_TXT))->GetCheck())
	{
		m_iSelType = 0;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_DOC))->GetCheck())
	{
		m_iSelType = 1;
	}
	else
	{
		AfxMessageBox(_T("请至少选择一种提取文件类型！"));
		return ;
	}
	CDialogEx::OnOK();
}
