// DlgFM.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgFM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFM dialog


CDlgFM::CDlgFM(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFM)
	m_channel = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFM)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_CHANNEL, m_channel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFM, CDialog)
	//{{AFX_MSG_MAP(CDlgFM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFM message handlers

void CDlgFM::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
