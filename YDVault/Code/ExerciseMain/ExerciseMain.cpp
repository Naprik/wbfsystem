// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// ExerciseMain.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CExerciseMainApp 构造

CExerciseMainApp::CExerciseMainApp()
{

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pDatabase = NULL;
	m_pFtpRef = new CFtpRef();
	m_bInit = FALSE;
}
CExerciseMainApp::~CExerciseMainApp()
{
	CPtrAutoClean<CDatabaseEx> clr1(m_pDatabase);
	CPtrAutoClean<CFtpRef> clr2(m_pFtpRef);
}


// 唯一的一个 CExerciseMainApp 对象

CExerciseMainApp theApp;


// CExerciseMainApp 初始化

BOOL CExerciseMainApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED );
	//初始化配置参数
	
	CString strMainPath;
	CFilePathHelper::GetMainCfgPath(strMainPath);

	//CString strCfgPath = strMainPath + TEXT("\\exerciseCfg");
	//CString strPath = strCfgPath + TEXT("\\WorkspaceCfg.xml");
	//CWorkSpaceMgr::Instance()->Load(strPath);
	//
	//CString strWorkspace;
	//CWorkSpaceMgr::Instance()->GetWorkSpace(&strWorkspace);
	//判断是否需要拷贝log和cfg
	
	
	//加载抽题配置
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

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxInitRichEdit2();
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("我佰分练习系统"));
	LoadStdProfileSettings(0);  // 加载标准 INI 文件选项

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();

 	m_pDatabase = new CDatabaseEx();
 	HRESULT hr = E_FAIL;
 	hr = m_pDatabase->Open();
	if(FAILED(hr))
	{
		AfxMessageBox(_T("打开数据库失败！"));
		//DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
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

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ExerciseMainTYPE,
		RUNTIME_CLASS(CExerciseMainDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CObjPropertyView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	m_pDocTemplate = pDocTemplate;
	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生


	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
 	if (!ProcessShellCommand(cmdInfo))
 		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(SW_MAXIMIZE);
	pMainFrame->UpdateWindow();
	m_bInit = TRUE;
	pMainFrame->SetFirstSelect();
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CExerciseMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CExerciseMainApp 自定义加载/保存方法

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

// CExerciseMainApp 消息处理程序


int CExerciseMainApp::ExitInstance()
{
	CoUninitialize();
	BCGCBProCleanUp();
	return CWinAppEx::ExitInstance();
}
