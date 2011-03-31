// DlgSingleroll.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgSingleroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSingleroll dialog


CDlgSingleroll::CDlgSingleroll(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSingleroll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSingleroll)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSingleroll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSingleroll)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_PHOTO, m_photo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSingleroll, CDialog)
	//{{AFX_MSG_MAP(CDlgSingleroll)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSingleroll message handlers
BOOL CDlgSingleroll::OnInitDialog()
{
	CDialog::OnInitDialog();
	CButton   *aa=(CButton*)GetDlgItem(IDC_PHOTO);  
	CString path = m_path;
	HBITMAP   hbmp=(HBITMAP)::LoadImage(NULL,path,IMAGE_BITMAP,180,200,LR_LOADFROMFILE);   
	 aa->SetBitmap(hbmp); 

	GetDlgItem(IDC_STATIC12)->SetWindowText(m_id);
	GetDlgItem(IDC_STATIC1)->SetWindowText(m_name);
	GetDlgItem(IDC_STATIC2)->SetWindowText(m_gender);
	GetDlgItem(IDC_STATIC3)->SetWindowText(m_depart);
	GetDlgItem(IDC_STATIC4)->SetWindowText(m_suject);
	GetDlgItem(IDC_STATIC5)->SetWindowText(m_addr);
	GetDlgItem(IDC_STATIC6)->SetWindowText(m_start);
	GetDlgItem(IDC_STATIC7)->SetWindowText(m_end);

	return true;
}
