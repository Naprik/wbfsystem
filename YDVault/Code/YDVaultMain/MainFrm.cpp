
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "../ObjRef\YDObjectRef.h"
#include "YDVaultMain.h"
#include "../YDFormUIBase/ObjPropertyView.h"
#include "MainFrm.h"
#include "../YDFormUIBase/ObjPropertySheet.h"
#include "../Base\DataHandler.h"
#include "ChildFrm.h"
#include "../Base/LanguageTranslator.h"
#include "../base/AutoClean.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../YDFormUIBase/ObjPropSheetManager.h"
#include "CfgTree.h"
#include "../YDFormUIBase/ObjPropShow.h"
#include "../Include/YDExamComm.h"
#include "../YDUIExamOperate\RecievePaperHelper.h"
#include "AddPageHelper.h"
#include "../ydcom\StaticYdComApp.h"
#include "../Include\YDVaultError.h"
#include "../Include\ShowErr.h"
#include "../\YDFormUIBase\YdFormView.h"
#include "../YdCom\SearchDevCom.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_MESSAGE(WM_YD_OPEN_OBJECT,OnOpenObject)
	ON_MESSAGE(WM_GET_MSG_DICTIONARY,OnGetTranslator)
	ON_MESSAGE(WM_YD_OPEN_DOCUMENT,OnOpenDocument)
	ON_MESSAGE(WM_YD_GET_PROPSHEETMANAGER,OnGetPropSheetManager)
	ON_MESSAGE(WM_YD_CLOSE_ALL_PROP,OnCloseAllPropSheet)
	ON_MESSAGE(WM_YD_CLOSE_PROP,OnClosePropSheet)
	ON_MESSAGE(WM_YD_OPEN_OBJECT_EX, OnOpenOBjectEX)
	ON_MESSAGE(WM_YD_OPEN_DOCUMENT_EX, OnOpenDocumentEX)	
	ON_MESSAGE(WM_YD_GET_FTPREF,OnGetFtpRef)
	ON_MESSAGE(WM_YD_GET_DB,OnGetDB)
	ON_MESSAGE(WM_YD_GET_BHICOLORICONS,OnGetBHiColorIcons)
	ON_MESSAGE(WM_YD_ADD_PAGE,OnAddPage)
	ON_MESSAGE(WM_YD_GET_EXAM_MSG_LISTBOX,OnGetExamMsgListBox)
	ON_MESSAGE(WM_YD_GETLAST_SHEET,OnGetLastSheet)
	ON_MESSAGE(WM_YD_TEACHERCOM_BREAK,OnTeacherComBreak)
	
	

	//////////////////////////////////////////////////////////////////////////
	//Exam
	ON_MESSAGE(WM_YD_EXAM_OPEN_DEPARTMENT, OnOpenExamDepartment)	
	//////////////////////////////////////////////////////////////////////////
	
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
	m_pTranslator = new CLanguageTranslator();
	m_pPropSheetManager = new CObjPropSheetManager();
	m_pLastSheet = NULL;
}

CMainFrame::~CMainFrame()
{
	CPtrAutoClean<CLanguageTranslator> clr(m_pTranslator);
	CPtrAutoClean<CObjPropSheetManager> clr2(m_pPropSheetManager);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}



	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	CCfgTree CfgTree;
	CfgTree.Init();
	if (!CreateDockingWindows(&CfgTree))
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	
 



	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);
	lstBasicCommands.AddTail(ID_SYS_RELOGIN);
	lstBasicCommands.AddTail(ID_SYS_USERMRG);
	lstBasicCommands.AddTail(ID_SYS_USER_MODIFY_PWD);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows(CCfgTree* _pCfgTree)
{
	BOOL bNameValid;

	// Create class view
	CDockablePane* pFirstDockablePane = NULL;

	// Create file view

	if(_pCfgTree->m_bShowQuestionTree)
	{
		CString strConstQuestionManagement;
		bNameValid = strConstQuestionManagement.LoadString(IDS_YDVAULTMAIN_QUESTION_MANAGEMENT);
		ASSERT(bNameValid);
		if (!m_wndQuestionTreeView.Create(strConstQuestionManagement, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_QUESTION_TREE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create File View window\n");
			return FALSE; // failed to create
		}
		m_wndQuestionTreeView.EnableDocking(CBRS_ALIGN_ANY);
		pFirstDockablePane = &m_wndQuestionTreeView;
	}

	
	if(_pCfgTree->m_bShowPaperTree)
	{
		CString strConstPaperView;
		bNameValid = strConstPaperView.LoadString(IDS_YDVAULTMAIN_PAPER_MANAGEMENT);
		ASSERT(bNameValid);
		if (!m_wndPaperTreeView.Create(strConstPaperView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PAPER_TREE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Class View window\n");
			return FALSE; // failed to create
		}
		m_wndPaperTreeView.EnableDocking(CBRS_ALIGN_ANY);
		if(!pFirstDockablePane)
		{
			pFirstDockablePane = &m_wndPaperTreeView;
		}
		
	}


	if(_pCfgTree->m_bShowExamTree)
	{
		//Creat Exam Handle view
		CString strConstExamManagement;
		bNameValid = strConstExamManagement.LoadString(IDS_YDVAULTMAIN_EXAM_MANAGEM);
		ASSERT(bNameValid);
		if (!m_wndExamTreeView.Create(strConstExamManagement, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_EXAM_TREE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create File View window\n");
			return FALSE; // failed to create
		}
		m_wndExamTreeView.EnableDocking(CBRS_ALIGN_ANY);
		if(!pFirstDockablePane)
		{
			pFirstDockablePane = &m_wndExamTreeView;
		}
		
	
		

	}

	if(_pCfgTree->m_bShowDataMaintainTree)
	{
		//Creat DataMaintain view
		CString strConstDataMaintain;
		bNameValid = strConstDataMaintain.LoadString(IDS_YDVAULTMAIN_DATA_MAINTAIN);
		ASSERT(bNameValid);
		if (!m_wndDataMaintainTreeView.Create(strConstDataMaintain,
												this,
												CRect(0, 0, 200, 200),
												TRUE,
												ID_VIEW_DATAMAINTAIN_TREE,
												WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI)
												)
		{
			TRACE0("Failed to create File View window\n");
			return FALSE; // failed to create
		}
		m_wndDataMaintainTreeView.EnableDocking(CBRS_ALIGN_ANY);
		if(!pFirstDockablePane)
		{
			pFirstDockablePane = &m_wndDataMaintainTreeView;
		}
	}
	

	if(!pFirstDockablePane)
	{
		return TRUE;
	}

	SetDockingWindowIcons(_pCfgTree,theApp.m_bHiColorIcons);
	
	DockPane(pFirstDockablePane);
	if(_pCfgTree->m_bShowPaperTree && pFirstDockablePane != &m_wndPaperTreeView)
	{
		CDockablePane* pTabbedBar = NULL;
		m_wndPaperTreeView.AttachToTabWnd(pFirstDockablePane, DM_SHOW, TRUE, &pTabbedBar);
	}
	
	if(_pCfgTree->m_bShowExamTree && pFirstDockablePane != &m_wndExamTreeView)
	{
		CDockablePane* pTabbedBar1 = NULL;
		m_wndExamTreeView.AttachToTabWnd(pFirstDockablePane, DM_SHOW, TRUE, &pTabbedBar1);
	}


	if(_pCfgTree->m_bShowDataMaintainTree && pFirstDockablePane != &m_wndDataMaintainTreeView)
	{
		CDockablePane* pTabbedBar1 = NULL;
		m_wndDataMaintainTreeView.AttachToTabWnd(pFirstDockablePane, DM_SHOW, TRUE, &pTabbedBar1);
	}

	//Creat MsgDlgView view
	CString strConstMsg = _T("输出消息");
	//bNameValid = strConstDataMaintain.LoadString();
	ASSERT(bNameValid);
	if (!m_wndMsgDlgView.Create(strConstMsg, 
								this,
								CRect(0, 0, 200, 200), 
								TRUE,
								ID_VIEW_MSG_DLG,
								WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI)
								)
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}
	m_wndMsgDlgView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMsgDlgView);
	
	
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(CCfgTree* _pCfgTree,BOOL bHiColorIcons)
{
	if(_pCfgTree->m_bShowQuestionTree)
	{
		HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndQuestionTreeView.SetIcon(hFileViewIcon, FALSE);

	}
	if(_pCfgTree->m_bShowPaperTree)
	{
		HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndPaperTreeView.SetIcon(hClassViewIcon, FALSE);

	}
	
	if(_pCfgTree->m_bShowExamTree)
	{
		HICON hExamViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndExamTreeView.SetIcon(hExamViewIcon, FALSE);

	}


	UpdateMDITabbedBarsIcons();
}

// CMainFrame diagnostics

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


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
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

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

HRESULT CMainFrame::OnOpenObject(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	if(theApp.m_pMainWnd == NULL)
	{
		return S_OK;
	}
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)wParam;
	//if(pObjWrapper == NULL)
	//{
	//	return S_OK;
	//}
	OPERATION nOp  = (OPERATION)lParam;
	_ASSERT(nOp == OP_NEW || nOp==OP_EDIT || nOp == OP_VIEW );
	CObjPropertySheet* pSheet = m_pPropSheetManager->IsExist(pObjWrapper);
	if(pSheet != NULL)
	{
		OPERATION type;
		pSheet->GetOPERATION(type);
		if(type == nOp)
		{
			CMDIChildWnd* pChildFrm = (CMDIChildWnd*)pSheet->GetParentView()->GetParent();
			_ASSERT(pChildFrm!=NULL && pChildFrm->IsKindOf(RUNTIME_CLASS(CChildFrame)));
			pChildFrm->MDIActivate();
			return 0l;
		}		
	}	
	hr = m_pPropSheetManager->Show(pObjWrapper,nOp,pSheet);	
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CMainFrame::OnOpenOBjectEX(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	if(theApp.m_pMainWnd == NULL)
	{
		return S_OK;
	}
	COpenObjExParam* pOpenObjExParam = (COpenObjExParam*)wParam;
	ASSERT(pOpenObjExParam);
	_ASSERT(pOpenObjExParam->m_Op == OP_NEW ||
			pOpenObjExParam->m_Op==OP_EDIT || 
			pOpenObjExParam->m_Op == OP_VIEW );
   if(pOpenObjExParam->m_strName.CompareNoCase(EXAM_BULETOOTH_RECEIVE_PAPER) == 0 ||
			pOpenObjExParam->m_strName.CompareNoCase(EXAM_REDOUT_RECEIVE_PAPER) == 0 ||
			pOpenObjExParam->m_strName.CompareNoCase(EXAM_USB_RECEIVE_PAPER) == 0 
			)
	{
		CRecievePaperHelper RecievePaperHelper;
		hr = RecievePaperHelper.PrepareRecievePaper();
		if(FAILED(hr))
		{
			return hr;
		}
		if(hr == S_FALSE)
		{
			//说明不要显示下面的对话框
			return S_FALSE;
		}
	}
	else if(pOpenObjExParam->m_strName.CompareNoCase(EXAM_EXAM_BACK) == 0 )
	{
		if(!pGExamStruct->m_pExamDetail->m_bIsRollDone)
		{
			AfxMessageBox(_T("本场考试没有结束，无法对本场考试备份！"));
			return S_FALSE;
		}
	}
	else if(pOpenObjExParam->m_strName.CompareNoCase(EXAM_CREATE_PAPER_FILE) == 0)
	{
		CString msg;
		msg.Format(_T("确定需要生成本场考试文件？"));
		if ( MessageBox(msg,_T("确认"),MB_OKCANCEL) != IDOK)
		{
			return S_FALSE;
		}
		//m_examdeteil. nRoll_pattern= 1;
		//pGExamStruct->m_pExamDetail->m_nSetp = 1;
	}
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
	if(pOpenObjExParam->m_bUseStaicStruct)
	{
		UINT uPageCount = pSheet->GetPageCount();
		for(UINT i = 0; i < uPageCount;i++)
		{
			CYdFormView* pFormView = pSheet->GetPage(i);
			ASSERT(pFormView);
			hr = pFormView->ExeSpecial();
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}
	
	return S_OK;
}

HRESULT CMainFrame::OnOpenDocument(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	m_pLastSheet = NULL;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)wParam;
	CString strTitle;
	if(pObjWrapper)
	{
		CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
		OBJID id = ID_EMPTY;
		hr = pObjRef->GetID(&id);
		if(FAILED(hr))
		{
			return NULL;
		}
		if(id == ID_EMPTY)
		{
			CString strDBName;
			hr = pObjRef->GetDBName(strDBName);
			if(FAILED(hr))
			{
				return NULL;
			}
			if(strDBName.CompareNoCase(DB_QVAULTCLASSIFICATION) == 0)
			{
				strTitle = _T("新建题库大类");
			}
			else if(strDBName.CompareNoCase(DB_QUESTIONVAULT) == 0)
			{
				strTitle = _T("新建题库");
			}
		}
		else
		{
			hr = pObjRef->GetPropVal(L"Name",strTitle);
			if(FAILED(hr))
			{
				return NULL;
			}
		}
		
	}
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
	hr = pObjPropView->CreateSheet(pObjWrapper,nOp);
	if(FAILED(hr))
	{
		return NULL;
	}

	pDoc->SetTitle(strTitle);
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

inline HRESULT CMainFrame::OnGetTranslator(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)m_pTranslator;
}

HRESULT CMainFrame::OnGetPropSheetManager(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)m_pPropSheetManager;
}

HRESULT CMainFrame::OnGetFtpRef(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)theApp.m_pFtpRef;
}

HRESULT CMainFrame::OnGetDB(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)theApp.m_pDatabase;
}

HRESULT CMainFrame::OnGetBHiColorIcons(WPARAM wParam,LPARAM)
{
	return (HRESULT)theApp.m_bHiColorIcons;
}

HRESULT CMainFrame::OnCloseAllPropSheet(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr= E_FAIL;
	while(m_pPropSheetManager->GetSheetObjWraperCount() > 0 )
	{
		int iPrevCount = m_pPropSheetManager->GetSheetObjWraperCount(); 
		CObjPropertySheet* pSheet = NULL;
		CYdObjWrapper* pObjWrapper = NULL;
		m_pPropSheetManager->GetSheetObjWraper(0,pSheet,pObjWrapper);
		if(pSheet != NULL)
		{
			hr = pSheet->GetParentView()->GetParent()->SendMessage(WM_CLOSE);
			if(FAILED(hr))
			{
				return hr;
			}
			int iAfterCount = m_pPropSheetManager->GetSheetObjWraperCount();
			if(iAfterCount == iPrevCount)
			{
				//说明当前窗口没有关闭，不能关闭
				return S_FALSE;
			}
		}
	}
	while(m_pPropSheetManager->GetSheetStrCount() > 0 )
	{
		int iPrevCount = m_pPropSheetManager->GetSheetStrCount(); 
		CObjPropertySheet* pSheet = NULL;
		CString str ;
		m_pPropSheetManager->GetSheetStr(0,pSheet,str);
		if(pSheet != NULL)
		{
			hr = pSheet->GetParentView()->GetParent()->SendMessage(WM_CLOSE);
			if(FAILED(hr))
			{
				return hr;
			}
			int iAfterCount = m_pPropSheetManager->GetSheetStrCount();
			if(iAfterCount == iPrevCount)
			{
				//说明当前窗口没有关闭，不能关闭
				return S_FALSE;
			}
		}
	}
	
	return S_OK;
}

HRESULT CMainFrame::OnClosePropSheet(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr= E_FAIL;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)wParam;
	ASSERT(pObjWrapper);
	for(int i = 0;i < m_pPropSheetManager->GetSheetObjWraperCount();i++)
	{
		CObjPropertySheet* pSheet = NULL;
		CYdObjWrapper* pWrapper = NULL;
		m_pPropSheetManager->GetSheetObjWraper(i,pSheet,pWrapper);
		if(pSheet != NULL &&pWrapper == pObjWrapper )
		{
			pSheet->GetParentView()->GetParent()->SendMessage(WM_CLOSE);
			return S_OK;
		}
	}

	return S_OK;
}

HRESULT CMainFrame::OnOpenExamDepartment(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	AfxMessageBox(_T("OnOpenExamDepartment"));
	return S_OK;
}


HRESULT CMainFrame::OnTeacherComBreak(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr= E_FAIL;
	CYDTeacherAppCom* pAppCom = (CYDTeacherAppCom*)wParam;
	ASSERT(pAppCom);
	while(TRUE)
	{
		CString strMsg;
		strMsg.Format(_T("在端口%s的教师机%s断开，请重新插入!"),pAppCom->m_strComPort,pAppCom->m_strMac);
		AfxMessageBox(strMsg,MB_OK|MB_ICONINFORMATION);
		CSearchDevCom SearchDevCom;
		std::list<CString> lstSearchedCom;
		hr = SearchDevCom.SearchDev(lstSearchedCom);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return hr;
		}
		BOOL bFind = FALSE;
		for(std::list<CString>::const_iterator itr = lstSearchedCom.begin();
			itr != lstSearchedCom.end();++itr)
		{
			CString strPort = (*itr);
			if (strPort.GetLength() > 4) //如果端口号大于"com10"，需要在端口名称前面增加“\\\\.\\”
			{
				strPort = _T("\\\\.\\") + strPort;
			}
			if(pAppCom->m_strComPort.CompareNoCase(strPort) == 0)
			{
				bFind = TRUE;
				break;
			}
		}
		if(bFind)
		{
			break;
		}
	}
	return S_OK;
	
	return S_OK;
}

HRESULT CMainFrame::OnAddPage(WPARAM wParam,LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	CObjPropShow* pObjPropShow = (CObjPropShow*)wParam;
	ASSERT(pObjPropShow);
	CAddPageHelper Helper;
	hr = Helper.AddPage(pObjPropShow);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CMainFrame::OnGetExamMsgListBox(WPARAM wParam,LPARAM lParam)
{
	return (HRESULT)(&m_wndMsgDlgView.m_lstBoxmsg);
}

HRESULT CMainFrame::OnGetLastSheet(WPARAM wParam,LPARAM lParam)
{
	return (HRESULT)m_pLastSheet;
}
void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(pGExamStruct->m_pExamDetail->m_nSetp == YDEXAM_STEP_BGGINEXAM)
	{
		AfxMessageBox(_T("已经进入开考状态，系统不能退出！"));
		return;
	}
	if(IDYES != AfxMessageBox(_T("确定要退出系统吗？"),MB_YESNO|MB_ICONQUESTION))
	{
		return;
	}
	hr = SendMessage(WM_YD_CLOSE_ALL_PROP,0,0);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(hr == S_FALSE)
	{
		return;
	}
	CMDIFrameWndEx::OnClose();
}

