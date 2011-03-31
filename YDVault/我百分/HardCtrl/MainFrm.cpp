#include "stdafx.h"
#include "HardCtrl.h"
#include "MainFrm.h"
#include "MenuEx.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SHOWLEFTBAR, OnShowleftbar)
	ON_COMMAND(ID_SHOWBOTTOMBAR, OnShowbottombar)
	ON_UPDATE_COMMAND_UI(ID_SHOWLEFTBAR, OnUpdateShowleftbar)
	ON_UPDATE_COMMAND_UI(ID_SHOWBOTTOMBAR, OnUpdateShowbottombar)
	ON_COMMAND(IDM_MAIN, OnMain)
	ON_COMMAND(IDM_EXAM, OnShowExam)
	ON_COMMAND(IDM_DATE, OnShowDate)
	ON_WM_INITMENU()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
}
void CMainFrame::OnShowleftbar() 
{//
	ShowControlBar(&m_wndLeft, !m_wndLeft.IsVisible(), FALSE);
}
void CMainFrame::OnShowbottombar() 
{
//	ShowControlBar(&m_wndBottom, !m_wndBottom.IsVisible(), FALSE);	
}
void CMainFrame::OnUpdateShowleftbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndLeft.IsVisible());		
}
void CMainFrame::OnUpdateShowbottombar(CCmdUI* pCmdUI) 
{
//	pCmdUI->SetCheck(m_wndBottom.IsVisible());		
}
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_dlgMain.Create(IDD_FORM_MAIN, this))
		return -1;
	m_dlgMain.SetDlgCtrlID(AFX_IDW_PANE_FIRST|AFX_WS_DEFAULT_VIEW);
    m_dlgMain.ShowWindow( TRUE );
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return -1;      
	if (!m_wndStatusBar.Create(this) ||!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
		return -1;      
	if (!m_wndLeft.Create(_T("控制窗口"), this, CSize(250,260),TRUE,123))
		return -1;
	if (!m_wndBottom.Create(_T("消息窗口"), this,CSize(200,130),TRUE,124))
		return -1;	
	m_TabCtrl.Create(TCS_MONOSPACE|TCS_ANIMATE|TCS_DOWN|WS_CHILD|WS_VISIBLE,CRect(0,0,100,100),&m_wndLeft,125);
	
	m_dlgMsg.Create(IDD_DLG_MSG,&m_wndBottom);
    m_dlgMsg.ShowWindow( SW_SHOW );
	
	m_wndToolBar.SetButtonText(0,"主窗口");
	
	m_wndToolBar.SetButtonText(2,"考试操作");
	m_wndToolBar.SetButtonText(3,"数据查询");
	m_wndToolBar.SetButtonText(5,"退出");
	
	CRect rc(0, 0, 0, 0);
	CSize sizeMax(0, 0);
	CToolBarCtrl& bar = m_wndToolBar.GetToolBarCtrl();
	for (int nIndex = bar.GetButtonCount() - 1; nIndex >= 0; nIndex--)
	{
		bar.GetItemRect(nIndex, rc);
		
		rc.NormalizeRect();
		sizeMax.cx = __max(rc.Size().cx, sizeMax.cx);
		sizeMax.cy = __max(rc.Size().cy, sizeMax.cy);
	}
	m_wndToolBar.SetSizes(sizeMax, CSize(42,32)); 
	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	m_wndLeft.SetBarStyle(m_wndLeft.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndBottom.SetBarStyle(m_wndBottom.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndLeft.EnableDocking(CBRS_ALIGN_ANY);
	m_wndBottom.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndLeft, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndBottom, AFX_IDW_DOCKBAR_BOTTOM);
	
	m_TabCtrl.AddPage(RUNTIME_CLASS(CLeftAudit),CLeftAudit::IDD,"考试操作",IDI_CTRL);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CLeftReport),CLeftReport::IDD,"数据报表",IDI_WEB);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CLeftManage),CLeftManage::IDD,"信息查询",IDI_CONFIG);
	
	m_TabCtrl.UpdateWindow();
	
	RECT rect;
	::GetWindowRect(::GetDesktopWindow(),&rect);
	rect.left = 0;
	rect.top  = 0;
	rect.right  = (rect.right-rect.left)* 100/100;
	rect.bottom = (rect.bottom-rect.top)* 100/100;
	MoveWindow(&rect);
	
	m_menu.SetImageLeft(IDB_BITMAP1);
	m_menu.SetHighLightColor(RGB(0,0,127)); 
	m_menu.SetTextColor(RGB(0,0,0)); 
	//	m_menu.SetBackColor(RGB(143,167,207)); 
	ShowWindow(SW_MAXIMIZE);
	return 0;
}
void CMainFrame::OnMain() 
{
     m_dlgMain.ShowDlg(0);	
}
void CMainFrame::OnShowExam()
{
	m_TabCtrl.SetActivePage(0);
}
void CMainFrame::OnShowDate()
{
	m_TabCtrl.SetActivePage(1);
}

void CMainFrame::OnInitMenu(CMenu* pMenu) 
{
	CFrameWnd::OnInitMenu(pMenu);
}
void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if(!nIDCtl) m_menu.MeasureItem(lpMeasureItemStruct);
	CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(!nIDCtl) m_menu.DrawItem(lpDrawItemStruct);
	CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);

}

