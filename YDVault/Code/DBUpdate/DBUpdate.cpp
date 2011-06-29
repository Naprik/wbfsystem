
// DBUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DBUpdate.h"
#include "DBUpdateDlg.h"
#include "../DBBase/DatabaseEx.h"
#include "../MainUIBase/DlgLoginEX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBUpdateApp

BEGIN_MESSAGE_MAP(CDBUpdateApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDBUpdateApp construction

CDBUpdateApp::CDBUpdateApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDBUpdateApp object

CDBUpdateApp theApp;


// CDBUpdateApp initialization

BOOL CDBUpdateApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxOleInit();
	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	m_pDatabase = new CDatabaseEx();
 	HRESULT hr = E_FAIL;
 	hr = m_pDatabase->Open();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("�����ݿ�ʧ�ܣ�"));
		//DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	
	CDlgLoginEX dlgLogin;
	
	dlgLogin.m_pDatabase = m_pDatabase;
	if(dlgLogin.DoModal() != IDOK)
	{
		return FALSE;
	}


	CDBUpdateDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_pDatabase = m_pDatabase;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

