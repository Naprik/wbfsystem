
// YDVaultMain.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "YDVaultMain.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "YDVaultMainDoc.h"
#include "YDVaultMainView.h"
#include "../YDFormUIBase/ObjPropertyView.h"
#include "../DBBase/DatabaseEx.h"
#include "../FtpBase/FtpRef.h"
#include "../Base/AutoClean.h"
#include "../Include/ShowErr.h"
#include "../MainUIBase/DlgLoginEX.h"
#include "../YDUIUserManagement\DlgUserMgr.h"
#include "../ObjRef\YDUserRef.h"
#include "../YDUIUserManagement\StaticYdUser.h"
#include "../YDUIUserManagement\DlgModifyPwd.h"
#include "../YdCom\StaticYdComApp.h"
#include "../YDUIUserManagement/AuthorityHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYDVaultMainApp

BEGIN_MESSAGE_MAP(CYDVaultMainApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CYDVaultMainApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_SYS_USERMRG, &CYDVaultMainApp::OnSysUsermrg)
	ON_COMMAND(ID_SYS_USER_MODIFY_PWD, &CYDVaultMainApp::OnSysUserModifyPwd)
	ON_COMMAND(ID_SYS_RELOGIN, &CYDVaultMainApp::OnSysRelogin)
END_MESSAGE_MAP()


// CYDVaultMainApp construction

CYDVaultMainApp::CYDVaultMainApp()
{

	m_bHiColorIcons = TRUE;
	m_pDatabase = new CDatabaseEx();
	m_pFtpRef = new CFtpRef();

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


CYDVaultMainApp::~CYDVaultMainApp()
{
	CPtrAutoClean<CDatabaseEx> clr1(m_pDatabase);
	CPtrAutoClean<CFtpRef> clr2(m_pFtpRef);
}
// The one and only CYDVaultMainApp object

CYDVaultMainApp theApp;


// CYDVaultMainApp initialization
TCHAR	 _ModulePath[MAX_PATH] ;
BOOL CYDVaultMainApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	


	CWinAppEx::InitInstance();
	DWORD nret = GetModuleFileName(theApp.m_hInstance,_ModulePath,MAX_PATH);
	_ModulePath[nret] = 0;
	TCHAR* p = _tcsrchr(_ModulePath,_T('\\'));
	if(p!=NULL) *p = 0;
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxInitRichEdit2(); 
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using th2ese features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("YDVault"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	
	HRESULT hr = E_FAIL;
	hr = m_pDatabase->Open();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("打开数据库失败！"));
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}

	hr = m_pFtpRef->Init();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("FTP连接失败！"));
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	
	CDlgLoginEX dlg;
	int argc = 0;
	PWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (argc == 5)
	{
		dlg.m_slientmode = TRUE;
		dlg.m_user = CString(argv[2]);
		dlg.m_pass = CString(argv[4]);
		HeapFree(GetProcessHeap(), 0, argv);
	}
	dlg.m_pDatabase = m_pDatabase;
	if(dlg.DoModal() != IDOK)
	{
		return FALSE;
	}
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_LOGIN, &bRight);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONQUESTION);
		return FALSE;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有登录题库的权限！"));
		return FALSE;
	}
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_YDVaultMainTYPE,
		RUNTIME_CLASS(CYDVaultMainDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CObjPropertyView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	m_pDocTemplate = pDocTemplate;



	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	cmdInfo.m_nShellCommand=CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	pGExamStruct->m_pMainWnd = AfxGetMainWnd();
	ASSERT(pGExamStruct->m_pMainWnd);
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CYDVaultMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CYDVaultMainApp customization load/save methods

void CYDVaultMainApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_YDVAULTMAIN_QUESTION_MANAGEMENT);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_MENU_QUESTION_VAULT);
	GetContextMenuManager()->AddMenu(strName, IDR_MENU_QUESTION_VAULT_CLASSIFICATION);
	GetContextMenuManager()->AddMenu(strName, IDR_MENU_QUESTION_TYPE);
}

void CYDVaultMainApp::LoadCustomState()
{
}

void CYDVaultMainApp::SaveCustomState()
{
}

// CYDVaultMainApp message handlers




int CYDVaultMainApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	BCGCBProCleanUp();
	return CWinAppEx::ExitInstance();
}


void CYDVaultMainApp::OnSysUsermrg()
{
	// TODO: Add your command handler code here
	//只有系统管理员才可以进行此操作
	HRESULT hr = E_FAIL;
	CYDUserRef* pCurUserRef = NULL;
	hr = CStaticYdUser::Instance()->GetCurUser(pCurUserRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	ASSERT(pCurUserRef);
	VARIANT_BOOL bAdmin = FALSE;
	hr = pCurUserRef->IsSysUser(&bAdmin);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(!bAdmin)
	{
		AfxMessageBox(_T("当前用户不是系统管理员，不能进行用户管理操作！"));
		return;
	}
	CDlgUserMgr dlg;
	dlg.DoModal();
}

void CYDVaultMainApp::OnSysUserModifyPwd()
{
	CDlgModifyPwd dlg;
	dlg.DoModal();
}

void CYDVaultMainApp::OnSysRelogin()
{
	CDlgLoginEX dlg;
	dlg.m_pDatabase = m_pDatabase;
	dlg.DoModal();
}
