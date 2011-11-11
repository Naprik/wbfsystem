// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// ExerciseMain.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "ExerciseMain.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ExerciseMainDoc.h"
#include "LeftView.h"

#include "WorkSpaceMgr.h"
#include "ExamSelectQuestionCfgMgr.h"
#include "ExerciseSelectQuestionCfgMgr.h"
#include "../Base\AutoClean.h"
#include "../DBBase\DatabaseEx.h"
#include "../MainUIBase\DlgLoginEX.h"
#include "../Base\FilePathHelper.h"
#include "../FtpBase\FtpRef.h"
#include "BasicView.h"
#include "../YDFormUIBase/ObjPropertyView.h"
#include "../YDUIUserManagement\StaticYdUser.h"
#include "../ObjRef/YDUserRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_CfgPath;
CString g_LogPath;
// CExerciseMainApp

BEGIN_MESSAGE_MAP(CExerciseMainApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CExerciseMainApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CExerciseMainApp ����

CExerciseMainApp::CExerciseMainApp()
{

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pDatabase = NULL;
	m_pFtpRef = new CFtpRef();
	m_bInit = FALSE;
}
CExerciseMainApp::~CExerciseMainApp()
{
	CPtrAutoClean<CDatabaseEx> clr1(m_pDatabase);
	CPtrAutoClean<CFtpRef> clr2(m_pFtpRef);
}


// Ψһ��һ�� CExerciseMainApp ����

CExerciseMainApp theApp;


// CExerciseMainApp ��ʼ��

BOOL CExerciseMainApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED );
	//��ʼ�����ò���
	
	CString strMainPath;
	CFilePathHelper::GetMainCfgPath(strMainPath);

	//CString strCfgPath = strMainPath + TEXT("\\exerciseCfg");
	//CString strPath = strCfgPath + TEXT("\\WorkspaceCfg.xml");
	//CWorkSpaceMgr::Instance()->Load(strPath);
	//
	//CString strWorkspace;
	//CWorkSpaceMgr::Instance()->GetWorkSpace(&strWorkspace);
	//�ж��Ƿ���Ҫ����log��cfg
	
	
	//���س�������
	//strWorkspace = _T("C:\\YDWork");
	g_CfgPath = strMainPath + TEXT("\\exerciseCfg");

	//CFilePathHelper::CopyFolder(strCfgPath,g_CfgPath,TRUE);
	CString strPath = g_CfgPath + TEXT("\\ExerciseQuestionCfg.xml");
	CExerciseSelectQuestionCfgMgr::Instance()->m_uSelQuesitonMode = SEL_QUESTION_EXERCISE;
	CExerciseSelectQuestionCfgMgr::Instance()->Load(strPath);
	strPath = g_CfgPath + TEXT("\\ExamQuestionCfg.xml");
	CExamSelectQuestionCfgMgr::Instance()->m_uSelQuesitonMode = SEL_QUESTION_EXAM;
	CExamSelectQuestionCfgMgr::Instance()->Load(strPath);

	g_LogPath = strMainPath + TEXT("\\exerciseLog");
	/*CString strLogPath = strMainPath + TEXT("\\exerciseLog");
	
	CFilePathHelper::CopyFolder(strLogPath,g_LogPath,TRUE);*/

	/*CString strDBPath = strMainPath + TEXT("\\DB");
	strPath = strWorkspace + TEXT("\\db");
	CFilePathHelper::CopyFolder(strDBPath, strPath,TRUE);*/

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxInitRichEdit2();
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("�Ұ۷���ϰϵͳ"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();

 	m_pDatabase = new CDatabaseEx();
 	HRESULT hr = E_FAIL;
 	hr = m_pDatabase->Open();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("�����ݿ�ʧ�ܣ�"));
		//DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	hr = m_pFtpRef->Init();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("FTP����ʧ�ܣ�"));
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	CDlgLoginEX dlg;
	dlg.m_user = L"";
	dlg.m_pDatabase = m_pDatabase;
	if(dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	CYDUserRef* pCurUserRef = NULL;
	hr = CStaticYdUser::Instance()->GetCurUser(pCurUserRef);
	if (FAILED(hr))
	{
		return FALSE;
	}
	CString strUser;
	pCurUserRef->GetPropVal(L"name", strUser);
	g_LogPath += L"\\";
	g_LogPath += strUser;
	

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ExerciseMainTYPE,
		RUNTIME_CLASS(CExerciseMainDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CObjPropertyView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	m_pDocTemplate = pDocTemplate;
	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
 	if (!ProcessShellCommand(cmdInfo))
 		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->UpdateWindow();
	m_bInit = TRUE;
	pMainFrame->SetFirstSelect();
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CExerciseMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CExerciseMainApp �Զ������/���淽��

void CExerciseMainApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CExerciseMainApp::LoadCustomState()
{
}

void CExerciseMainApp::SaveCustomState()
{
}

// CExerciseMainApp ��Ϣ�������


int CExerciseMainApp::ExitInstance()
{
	CoUninitialize();
	BCGCBProCleanUp();
	return CWinAppEx::ExitInstance();
}
