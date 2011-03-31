// DlgRollingChoose.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgRollingChoose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRollingChoose dialog


CDlgRollingChoose::CDlgRollingChoose(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRollingChoose::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRollingChoose)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgRollingChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRollingChoose)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRollingChoose, CDialog)
	//{{AFX_MSG_MAP(CDlgRollingChoose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRollingChoose message handlers

void CDlgRollingChoose::OnOK() 
{
	if(!(((CButton *)GetDlgItem(IDC_RADIO_LINK))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_OFFLIN))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_QUERY))->GetCheck()))
	{
		AfxMessageBox("没有选择本场考试收卷模式，无法进入下一步！");
		return;
	}
	
	if(((CButton *)GetDlgItem(IDC_RADIO_LINK))->GetCheck())
	{
		m_rollkind = 101;
	}	
	else if(((CButton *)GetDlgItem(IDC_RADIO_OFFLIN))->GetCheck())
	{
		m_rollkind = 201;
	}
	else
	{
		m_rollkind = 301;
	}
	
	
	CDialog::OnOK();
}
