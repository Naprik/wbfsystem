// PdemErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YdErrorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYdErrorDlg dialog


CYdErrorDlg::CYdErrorDlg(const CString& strMsg,HRESULT hr,UINT nIcon,CWnd* pParent /*=NULL*/)
	: CDialog(CYdErrorDlg::IDD, pParent),m_strMsg(strMsg),m_hr(hr),m_nIcon(nIcon)
{
	//{{AFX_DATA_INIT(CYdErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYdErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYdErrorDlg)
	DDX_Control(pDX, IDC_STATIC_PEDMERROR_ICON, m_ctrlIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYdErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CYdErrorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYdErrorDlg message handlers

BOOL CYdErrorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_STATIC_PDEMERROR_MSG)->SetWindowText(m_strMsg);
	
	if((m_nIcon&0xf0) == MB_ICONWARNING)
		m_ctrlIcon.SetIcon(::LoadIcon(NULL,IDI_WARNING));
	else if((m_nIcon&0xf0) == MB_ICONSTOP)
		m_ctrlIcon.SetIcon(::LoadIcon(NULL,IDI_HAND));
	else if((m_nIcon&0xf0) == MB_ICONERROR)
		m_ctrlIcon.SetIcon(::LoadIcon(NULL,IDI_HAND));
	else if((m_nIcon&0xf0) == MB_ICONQUESTION)
		m_ctrlIcon.SetIcon(::LoadIcon(NULL,IDI_QUESTION));

	if((m_nIcon&0x0f) == MB_OK)
	{
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		CRect rect,rect1;
		GetClientRect(&rect);
		GetDlgItem(IDOK)->GetWindowRect(&rect1);
		ScreenToClient(&rect1);
		int x = (rect.Width()-rect1.Width())/2;
		int y = x+rect1.Width();
		rect1.left = x;
		rect1.right = y;
		GetDlgItem(IDOK)->MoveWindow(&rect1);
	}
	else if((m_nIcon&0x0f) == MB_OKCANCEL)
	{
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	}
	else if((m_nIcon&0x0f) == MB_YESNO)
	{

		GetDlgItem(IDOK)->SetWindowText(_T("ÊÇ&Y"));		

		GetDlgItem(IDCANCEL)->SetWindowText(_T("·ñ&N"));
		
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CYdErrorDlg::DoModal() 
{
	//CExtDllState state;	
	return CDialog::DoModal();
}
