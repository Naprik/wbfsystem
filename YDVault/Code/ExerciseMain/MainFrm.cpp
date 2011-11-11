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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ExerciseMain.h"

#include "MainFrm.h"
#include "ExerciseMainDlg.h"
#include "DlgCfgExamTime.h"
#include "DlgSelQuestionCfg.h"
#include "ExamSelectQuestionCfgMgr.h"
#include "ExerciseSelectQuestionCfgMgr.h"
#include "ExamMainDlg.h"
#include "SelLogDlg.h"
#include "BasicView.h"
#include "PersionInfoView.h"
#include "HistoryInfoView.h"
#include "ChildFrm.h"
#include "UserInfoDlg.h"
#include "../YDFormUIBase/ObjPropertyView.h"
#include "../YDFormUIBase/ObjPropShow.h"
#include "../Base/AutoClean.h"
#include "../YDUIUserManagement/StaticYdUser.h"
#include "../ObjRef/YDUserRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)

	ON_COMMAND(ID_EM_NEW_EXAM, &CMainFrame::OnEmNewExam)
	ON_COMMAND(ID_EM_NEW_EXERCISE, &CMainFrame::OnEmNewExercise)
	ON_COMMAND(ID_EM_SET_EXAMTIME, &CMainFrame::OnEmSetExamtime)
	ON_COMMAND(ID_EM_SET_WORKSPACE, &CMainFrame::OnEmSetWorkspace)
	ON_COMMAND(ID_SET_PERSON_INFO, &CMainFrame::OnUpdateUserInfo)
	ON_COMMAND(ID_EM_LOAD_EXERCISE, &CMainFrame::OnEmLoadExercise)
	ON_COMMAND(ID_EM_SET_EXERCISE_CFG, &CMainFrame::OnEmSetExerciseCfg)
	ON_COMMAND(ID_EM_SET_EXAM_CFG, &CMainFrame::OnEmSetExamCfg)
	ON_COMMAND(ID_EM_LOADVAULT, &CMainFrame::OnLoadVault)
	ON_COMMAND(ID_EM_CLEARHISTORY, &CMainFrame::OnClearHistory)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_YD_GET_DB,OnGetDB)
	ON_MESSAGE(WM_YD_GET_FTPREF,OnGetFtpRef)
	ON_MESSAGE(WM_YD_ADD_PAGE,OnAddPage)
	ON_MESSAGE(WM_YD_OPEN_OBJECT_EX, OnOpenOBjectEX)
	ON_MESSAGE(WM_YD_OPEN_DOCUMENT_EX, OnOpenDocumentEX)
	ON_MESSAGE(WM_YD_GETLAST_SHEET,OnGetLastSheet)
	ON_MESSAGE(WM_YD_UPDATE_PERSIONINFO, OnUpdatePersionInfo)
	ON_MESSAGE(WM_YD_UPDATE_HISTORYINFO, OnUpdateHistroyInfo)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	m_pPropSheetManager = new CObjPropSheetManager();
	m_pLastSheet = NULL;
}

CMainFrame::~CMainFrame()
{
	CPtrAutoClean<CObjPropSheetManager> clr(m_pPropSheetManager);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	
	
	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	CDockablePane* pFirstDockablePane = NULL;

	CString strInfo(L"用户基本信息");
		
	if (!m_treeview.Create(strInfo, this, CRect(0, 0, 200, 200), TRUE, IDC_TREE_VIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}
	m_treeview.EnableDocking(CBRS_ALIGN_ANY);
	if(!pFirstDockablePane)
	{
		pFirstDockablePane = &m_treeview;
	}
	DockPane(pFirstDockablePane);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_MAIN);
	ASSERT(bNameValid);
// 
// 	// 加载面板图像:
// 	m_PanelImages.SetImageSize(CSize(16, 16));
// 	m_PanelImages.Load(IDB_BUTTONS);

	// 初始主按钮:
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

 	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW_EXERCISE);
 	ASSERT(bNameValid);
 	pMainPanel->Add(new CMFCRibbonButton(ID_EM_NEW_EXERCISE, strTemp, 0, 0));
 	bNameValid = strTemp.LoadString(IDS_RIBBON_LOAD_EXERCISE);
 	ASSERT(bNameValid);
 	pMainPanel->Add(new CMFCRibbonButton(ID_EM_LOAD_EXERCISE, strTemp, 1, 1));
 	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW_EXAM);
 	/*ASSERT(bNameValid);
 	pMainPanel->Add(new CMFCRibbonButton(ID_EM_NEW_EXAM, strTemp, 2, 2));*/
	ASSERT(bNameValid);
	bNameValid = strTemp.LoadString(IDS_RIBBON_LOAD_VAULT);
	ASSERT(bNameValid);
 	pMainPanel->Add(new CMFCRibbonButton(ID_EM_LOADVAULT, strTemp, 8, 8));

	bNameValid = strTemp.LoadString(IDS_RIBBON_CLEAR_HISTORY);
	ASSERT(bNameValid);
 	pMainPanel->Add(new CMFCRibbonButton(ID_EM_CLEARHISTORY, strTemp, 9, 9));
 	
	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_APP_EXIT, strTemp, 7, 7));


	// 为“操作”面板添加“主”类别:
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPERATOR);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryOp = m_wndRibbonBar.AddCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	// 创建“练习模式”面板:
	bNameValid = strTemp.LoadString(IDS_RIBBON_EXERCISE);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelExercise = pCategoryOp->AddPanel(strTemp, m_PanelImages.ExtractIcon(0));

	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW_EXERCISE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnNewExercise = new CMFCRibbonButton(ID_EM_NEW_EXERCISE, strTemp, 0, 0);
	pPanelExercise->Add(pBtnNewExercise);


	bNameValid = strTemp.LoadString(IDS_RIBBON_LOAD_EXERCISE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnLoadExercise = new CMFCRibbonButton(ID_EM_LOAD_EXERCISE, strTemp, 1, 1);
	pPanelExercise->Add(pBtnLoadExercise);

	//// 创建和添加“考试”面板:
	//bNameValid = strTemp.LoadString(IDS_RIBBON_EXAM);
	//ASSERT(bNameValid);
	//CMFCRibbonPanel* pPanelExam = pCategoryOp->AddPanel(strTemp, m_PanelImages.ExtractIcon (2));

	/*bNameValid = strTemp.LoadString(IDS_RIBBON_NEW_EXAM);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnExam = new CMFCRibbonButton(ID_EM_NEW_EXAM, strTemp, 2, 2);
	pPanelExam->Add(pBtnExam);*/

	// 为“设置”面板添加“主”类别:
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPTION);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategorySetting = m_wndRibbonBar.AddCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	// 创建“练习模式”面板:
	bNameValid = strTemp.LoadString(IDS_RIBBON_SETTING);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelSetting = pCategorySetting->AddPanel(strTemp, m_PanelImages.ExtractIcon(3));

// 	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_WORKSPACE);
// 	ASSERT(bNameValid);
// 	CMFCRibbonButton* pBtnWorkSpace = new CMFCRibbonButton(ID_EM_SET_WORKSPACE, strTemp, 3, 3);
// 	pPanelSetting->Add(pBtnWorkSpace);

	/*bNameValid = strTemp.LoadString(IDS_RIBBON_SET_TIME);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnTime = new CMFCRibbonButton(ID_EM_SET_EXAMTIME, strTemp, 4, 4);
	pPanelSetting->Add(pBtnTime);*/

	

	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_EXERCISECFG);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnExerciseCfg = new CMFCRibbonButton(ID_EM_SET_EXERCISE_CFG, strTemp, 5, 5);
	pPanelSetting->Add(pBtnExerciseCfg);

	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_PERSONINFO);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnTime = new CMFCRibbonButton(ID_SET_PERSON_INFO, strTemp, 4, 4);
	pPanelSetting->Add(pBtnTime);

	/*bNameValid = strTemp.LoadString(IDS_RIBBON_SET_EXAMCFG);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnExamCfg = new CMFCRibbonButton(ID_EM_SET_EXAM_CFG, strTemp, 5, 5);
	pPanelSetting->Add(pBtnExamCfg);*/

	// 将元素添加到选项卡右侧:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* 无默认命令*/, TRUE /* 右对齐*/);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// 添加快速访问工具栏命令:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_EM_NEW_EXERCISE);
	lstQATCmds.AddTail(ID_EM_LOAD_EXERCISE);
	lstQATCmds.AddTail(ID_EM_NEW_EXAM);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnEmNewExam()
{
	CExamMainDlg dlg(this);

	if (dlg.DoModal() == IDOK)
	{
	}
}

void CMainFrame::OnEmNewExercise()
{
 	CExerciseMainDlg dlg(this);
 	dlg.m_bLoadMode = FALSE;
 	if (dlg.DoModal() == IDOK)
 	{
 	}
}

void CMainFrame::OnEmSetExamtime()
{
	CDlgCfgExamTime dlg;
	dlg.DoModal();
}

void CMainFrame::OnEmSetWorkspace()
{
	AfxMessageBox(L"弹出设置工作目录对话框");
}

void CMainFrame::OnUpdateUserInfo()
{
	CUserInfoDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnEmLoadExercise()
{
	CSelLogDlg selDlg;
	if (selDlg.DoModal() == IDOK)
	{
		CExerciseMainDlg dlg(this);
		dlg.m_bLoadMode = TRUE;
		dlg.m_strLogName = selDlg.m_strFilename;
		if (dlg.DoModal() == IDOK)
		{
		}
	}
	
}

void CMainFrame::OnEmSetExerciseCfg()
{
	CDlgSelQuestionCfg dlg;
	dlg.m_pSelQCfgMgr = CExerciseSelectQuestionCfgMgr::Instance();
	dlg.DoModal();
}

void CMainFrame::OnEmSetExamCfg()
{
	CDlgSelQuestionCfg dlg;
	dlg.m_pSelQCfgMgr = CExamSelectQuestionCfgMgr::Instance();
	dlg.DoModal();
}
void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(IDYES != AfxMessageBox(_T("确定要退出系统吗？"),MB_YESNO|MB_ICONQUESTION))
	{
		return;
	}
	CMDIFrameWndEx::OnClose();
}
//HRESULT CMainFrame::OnOpenBasicInfoView(WPARAM wParam, LPARAM lParam)
//{
//	CMultiDocTemplate* pDocTemp = theApp.m_pDocTemplate;
//
//	ASSERT(pDocTemp != NULL);	
//	CDocument* pDoc = pDocTemp->OpenDocumentFile(NULL);
//	if (pDoc != NULL)
//	{
//		CMDIChildWnd* pChild = (CMDIChildWnd *) this->GetActiveFrame();
//		POSITION pos = pDoc->GetFirstViewPosition();
//		CView* pView = pDoc->GetNextView(pos);
//		_ASSERT(pView!=NULL);
//		ASSERT( pView->IsKindOf(RUNTIME_CLASS(CBasicView)) );
//		CBasicView* pTabView = (CBasicView*)pView;
//		pTabView->AddView(RUNTIME_CLASS (CPersionInfoView),L"用户基本信息",-1,NULL);
//	}
//
//	return S_OK;
//}
HRESULT CMainFrame::OnGetDB(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)theApp.m_pDatabase;
}
HRESULT CMainFrame::OnGetFtpRef(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)theApp.m_pFtpRef;
}

void CMainFrame::OnLoadVault()
{
	CYDUserRef* puser = NULL;
	CStaticYdUser::Instance()->GetCurUser(puser);
	CString commandline = L"YDVaultMain.exe ";
	commandline += L"-u \"";
	CString value;
	puser->GetPropVal(FIELD_YDUSER_NAME, value);
	commandline += value;
	commandline += L"\"";
	puser->GetPropVal(FIELD_YDUSER_PASSWORD, value);
	commandline += L" -p \"";
	commandline += value;
	commandline += L"\"";
	STARTUPINFO startinfo = {sizeof(startinfo)};
	PROCESS_INFORMATION processinfo;
	CreateProcess(NULL, (LPWSTR)(LPCTSTR)commandline, NULL, NULL,
		FALSE, 0, NULL, NULL, &startinfo, &processinfo);
	
}

void CMainFrame::OnClearHistory()
{
	CString logindex(_T(""));
	CFileFind find;
	CString logpath = g_LogPath;
	logpath += TEXT("\\exam\\*.xml");
	BOOL hasfile = find.FindFile(logpath);
	
	while (hasfile)
	{
		hasfile = find.FindNextFile();
		CString logname = find.GetFilePath();
		DWORD dwAttrs = GetFileAttributes(logname); 
		if ((dwAttrs & FILE_ATTRIBUTE_READONLY)) 
		{ 
			SetFileAttributes(logname, dwAttrs & ~FILE_ATTRIBUTE_READONLY); 
		}
		::DeleteFile(logname);
	}

	logpath = g_LogPath;
	logpath += TEXT("\\exercise\\*.xml");
	hasfile = find.FindFile(logpath);
	
	while (hasfile)
	{
		hasfile = find.FindNextFile();
		CString logname = find.GetFilePath();
		DWORD dwAttrs = GetFileAttributes(logname); 
		if ((dwAttrs & FILE_ATTRIBUTE_READONLY)) 
		{ 
			SetFileAttributes(logname, dwAttrs & ~FILE_ATTRIBUTE_READONLY); 
		}
		::DeleteFile(logname);
	}
	AfxGetMainWnd()->SendMessage(WM_YD_UPDATE_HISTORYINFO, (WPARAM)(&TREE_NODE_HISTORY),0);
}

HRESULT CMainFrame::SetFirstSelect()
{
	COpenObjExParam OpenObjExParam;
	OpenObjExParam.m_strName = TREE_NODE_USER_INFO;
	OpenObjExParam.m_Op = OP_VIEW;
	OpenObjExParam.m_bUseStaicStruct = FALSE;
	HRESULT hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT_EX,(WPARAM)(&OpenObjExParam),0);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return hr;
	}

	return S_OK;
}


HRESULT CMainFrame::OnAddPage(WPARAM wParam,LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	CObjPropShow* pObjPropShow = (CObjPropShow*)wParam;
	ASSERT(pObjPropShow);
	CObjPropertySheet* pSheet = pObjPropShow->m_pSheet;
	ASSERT(pSheet);
	if(pObjPropShow->m_type == SHOW_STRING)
	{
		if(pObjPropShow->m_strName.CompareNoCase(TREE_NODE_USER_INFO) == 0)
		{
			//user info
			pSheet->AddPage(RUNTIME_CLASS (CPersionInfoView), pObjPropShow->m_strName);
		}
		else if(pObjPropShow->m_strName.CompareNoCase(TREE_NODE_HISTORY) == 0)
		{
			pSheet->AddPage(RUNTIME_CLASS (CHistoryInfoView), pObjPropShow->m_strName);
		}
	}
	return S_OK;
}

HRESULT CMainFrame::OnOpenOBjectEX(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	
	if(theApp.m_pMainWnd == NULL)
	{
		return S_OK;
	}
	COpenObjExParam* pOpenObjExParam = (COpenObjExParam*)wParam;
	ASSERT(pOpenObjExParam);
	_ASSERT(pOpenObjExParam->m_Op == OP_NEW ||
			pOpenObjExParam->m_Op==OP_EDIT || 
			pOpenObjExParam->m_Op == OP_VIEW );
  
	
	CObjPropertySheet* pSheet = m_pPropSheetManager->IsExist(pOpenObjExParam->m_strName);
	if(pSheet != NULL)
	{
		OPERATION type;
		pSheet->GetOPERATION(type);
		if(type == pOpenObjExParam->m_Op)
		{
			CMDIChildWnd* pChildFrm = (CMDIChildWnd*)pSheet->GetParentView()->GetParent();
			_ASSERT(pChildFrm!=NULL && pChildFrm->IsKindOf(RUNTIME_CLASS(CChildFrame)));
			pChildFrm->MDIActivate();
		}

	}
	else
	{
		hr = m_pPropSheetManager->Show(pOpenObjExParam->m_strName, pOpenObjExParam->m_Op,pSheet);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	if(pSheet == NULL)
	{
		return S_OK;
	}
	
	return S_OK;
}

HRESULT CMainFrame::OnOpenDocumentEX(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	m_pLastSheet = NULL;
	CString *sTitle = (CString *)wParam;
	OPERATION nOp  = (OPERATION)lParam;

	CMultiDocTemplate* pDocTemp = theApp.m_pDocTemplate;

	ASSERT(pDocTemp != NULL);	
	CDocument* pDoc = pDocTemp->OpenDocumentFile(NULL);
	_ASSERT(pDoc != NULL);

	CMDIChildWnd* pChild = (CMDIChildWnd *) this->GetActiveFrame();
	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = pDoc->GetNextView(pos);
	_ASSERT(pView!=NULL);
	ASSERT( pView->IsKindOf(RUNTIME_CLASS(CObjPropertyView)) );
	CObjPropertyView* pObjPropView = (CObjPropertyView*)pView;
	hr = pObjPropView->CreateSheetEx(*sTitle,nOp);
	if(FAILED(hr))
	{
		return hr;
	}
	pDoc->SetTitle((LPCTSTR)(*sTitle));
	if(nOp == OP_VIEW)
	{
		((CChildFrame*)pChild)->ShowBelowBar(FALSE);
	}
	else
	{
		((CChildFrame*)pChild)->ShowBelowBar(TRUE);
	}
	((CChildFrame*)pChild)->UpdateBelowBar(nOp);
	pChild->RecalcLayout();
	m_pLastSheet = pObjPropView->GetPropertySheet();

	return S_OK;
}

HRESULT CMainFrame::OnGetLastSheet(WPARAM wParam,LPARAM lParam)
{
	return (HRESULT)m_pLastSheet;
}

HRESULT  CMainFrame::OnUpdatePersionInfo(WPARAM wParam,LPARAM lParam)
{
	CString *title = (CString *)wParam;
	CObjPropertySheet* psheet  = m_pPropSheetManager->IsExist(*title);
	if (psheet != NULL)
	{
		psheet->UpdateShow();
	}
	return S_OK;
}

HRESULT  CMainFrame::OnUpdateHistroyInfo(WPARAM wParam,LPARAM lParam)
{
	CString *title = (CString *)wParam;
	CObjPropertySheet* psheet  = m_pPropSheetManager->IsExist(*title);
	if (psheet != NULL)
	{
		psheet->UpdateShow();
	}
	return S_OK;
}