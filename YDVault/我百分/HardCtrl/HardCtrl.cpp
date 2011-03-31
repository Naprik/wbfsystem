#include "stdafx.h"
#include "HardCtrl.h"
#include "MainFrm.h"
#include "DlgLogin.h"

BEGIN_MESSAGE_MAP(CHardCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CHardCtrlApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CHardCtrlApp::CHardCtrlApp()
{
}
void CHardCtrlApp::SetXpStype(BOOL flag)
{
	CString ostype="";
	OSVERSIONINFOEX os;
	::ZeroMemory(&os,sizeof(os));
	os.dwOSVersionInfoSize=sizeof(os);
	::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&os));

	if(os.dwPlatformId==VER_PLATFORM_WIN32_NT && os.dwMajorVersion >=5)
	{
		if(os.dwMinorVersion==0)
			ostype = "Win2000";
		else
			ostype = "WinXP";
	}
	if(os.dwPlatformId ==VER_PLATFORM_WIN32_WINDOWS)
		ostype = "Win9x";
	

		typedef BOOL  (WINAPI *pClassXP)(HWND,BOOL);
		HINSTANCE hInst=NULL;
		hInst=::LoadLibrary("ClassXP.dll");
		if (hInst)
		{
			pClassXP pFunc= (pClassXP)GetProcAddress (hInst,"ClassXP");
			pFunc(NULL,flag);  
		}

}
CHardCtrlApp theApp;
BOOL CHardCtrlApp::InitInstance()
{
//	VERIFY( 1 == InitSkinMagicLib( AfxGetInstanceHandle(), _T("ReadUI") ,NULL,NULL ) );
//	VERIFY( 1 == LoadSkinFile("Skins\\xplus.smf")); 
//	VERIFY( 1 == SetDialogSkin( _T("Dialog") ) );
	AfxEnableControlContainer();
#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif
//     CDlgLogin dlg;
// 	if ( dlg.DoModal() !=IDOK )
// 		return FALSE;
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME);
	pFrame->SetIcon(LoadIcon(IDI_LOG),FALSE);
//	VERIFY( 1 == SetWindowSkin(m_pMainWnd->m_hWnd ,  _T("MainFrame") ) );

	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	SetXpStype(TRUE);


	return TRUE;
}
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	int		m_var;
	int		m_num;
	CString	m_msg;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_var = 0;
	m_num = 0;
	m_msg = _T("");
	//}}AFX_DATA_INIT
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VAR, m_var);
	DDX_Text(pDX, IDC_NUM, m_num);
	DDX_Text(pDX, IDC_MSG, m_msg);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CHardCtrlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
int CHardCtrlApp::ExitInstance() 
{
	CoUninitialize();
//	ExitSkinMagicLib();
	return CWinApp::ExitInstance();
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	
	m_msg = "\r\n    警告：本计算机程序受版权法和国际条约保护。未经授权复制或散播本计算机程序或其中的一部分，将受到严厉的民事或刑事处罚，并将在法律许可的范围内受到最大可能的起诉。";

	UpdateData( FALSE );
	return TRUE;  
}




