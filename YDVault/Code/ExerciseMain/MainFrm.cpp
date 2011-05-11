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

// MainFrm.cpp : CMainFrame ���ʵ��
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
	ON_COMMAND(ID_EM_LOAD_EXERCISE, &CMainFrame::OnEmLoadExercise)
	ON_COMMAND(ID_EM_SET_EXERCISE_CFG, &CMainFrame::OnEmSetExerciseCfg)
	ON_COMMAND(ID_EM_SET_EXAM_CFG, &CMainFrame::OnEmSetExamCfg)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_YD_GET_DB,OnGetDB)
	ON_MESSAGE(WM_YD_GET_FTPREF,OnGetFtpRef)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	
	
	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	CDockablePane* pFirstDockablePane = NULL;

	CString strInfo(L"�û�������Ϣ");
		
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_MAIN);
	ASSERT(bNameValid);
// 
// 	// �������ͼ��:
// 	m_PanelImages.SetImageSize(CSize(16, 16));
// 	m_PanelImages.Load(IDB_BUTTONS);

	// ��ʼ����ť:
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
 	ASSERT(bNameValid);
 	pMainPanel->Add(new CMFCRibbonButton(ID_EM_NEW_EXAM, strTemp, 2, 2));
 	
	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_APP_EXIT, strTemp, 7, 7));


	// Ϊ�������������ӡ��������:
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPERATOR);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryOp = m_wndRibbonBar.AddCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	// ��������ϰģʽ�����:
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

	// ��������ӡ����ԡ����:
	bNameValid = strTemp.LoadString(IDS_RIBBON_EXAM);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelExam = pCategoryOp->AddPanel(strTemp, m_PanelImages.ExtractIcon (2));

	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW_EXAM);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnExam = new CMFCRibbonButton(ID_EM_NEW_EXAM, strTemp, 2, 2);
	pPanelExam->Add(pBtnExam);

	// Ϊ�����á������ӡ��������:
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPTION);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategorySetting = m_wndRibbonBar.AddCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	// ��������ϰģʽ�����:
	bNameValid = strTemp.LoadString(IDS_RIBBON_SETTING);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelSetting = pCategorySetting->AddPanel(strTemp, m_PanelImages.ExtractIcon(3));

// 	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_WORKSPACE);
// 	ASSERT(bNameValid);
// 	CMFCRibbonButton* pBtnWorkSpace = new CMFCRibbonButton(ID_EM_SET_WORKSPACE, strTemp, 3, 3);
// 	pPanelSetting->Add(pBtnWorkSpace);

	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_TIME);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnTime = new CMFCRibbonButton(ID_EM_SET_EXAMTIME, strTemp, 4, 4);
	pPanelSetting->Add(pBtnTime);

	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_EXERCISECFG);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnExerciseCfg = new CMFCRibbonButton(ID_EM_SET_EXERCISE_CFG, strTemp, 5, 5);
	pPanelSetting->Add(pBtnExerciseCfg);

	bNameValid = strTemp.LoadString(IDS_RIBBON_SET_EXAMCFG);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnExamCfg = new CMFCRibbonButton(ID_EM_SET_EXAM_CFG, strTemp, 5, 5);
	pPanelSetting->Add(pBtnExamCfg);

	// ��Ԫ����ӵ�ѡ��Ҳ�:
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* ��Ĭ������*/, TRUE /* �Ҷ���*/);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// ��ӿ��ٷ��ʹ���������:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_EM_NEW_EXERCISE);
	lstQATCmds.AddTail(ID_EM_LOAD_EXERCISE);
	lstQATCmds.AddTail(ID_EM_NEW_EXAM);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

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
	AfxMessageBox(L"�������ù���Ŀ¼�Ի���");
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
	if(IDYES != AfxMessageBox(_T("ȷ��Ҫ�˳�ϵͳ��"),MB_YESNO|MB_ICONQUESTION))
	{
		return;
	}
	CMDIFrameWndEx::OnClose();
}

HRESULT CMainFrame::OnGetDB(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)theApp.m_pDatabase;
}
HRESULT CMainFrame::OnGetFtpRef(WPARAM wParam, LPARAM lParam)
{
	return (HRESULT)theApp.m_pFtpRef;
}