#include "stdafx.h"
#include "HardCtrl.h"
#include "DlgMsg.h"

IMPLEMENT_DYNCREATE(CDlgMsg, CDialog)
CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CDlgMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMsg)

	//}}AFX_DATA_INIT
}
void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMsg)
	DDX_Control(pDX, IDC_LIST1, m_msg);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CDlgMsg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CDlgMsg)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
BOOL CDlgMsg::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
//	HCURSOR hcur=::LoadCursorFromFile("AniCur.ani");           //加载动画广标
//	::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)hcur);
	AddSzControl(*GetDlgItem(IDC_LIST1),mdResize,mdResize);

	return TRUE; 
}
void CDlgMsg::ShowMsg(LPCTSTR lpszFormat, ...)
{
   m_msg.InsertString(0,lpszFormat);
}

HBRUSH CDlgMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(GetSysColor(COLOR_3DFACE));
		return (HBRUSH)GetSysColorBrush(COLOR_3DFACE);
	}	
	return hbr;
}

void CDlgMsg::ShowData(COleDateTime dt,CString domain)
{	
	UpdateData( FALSE );
}

void CDlgMsg::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingDialog::OnSize(nType, cx, cy);
}
