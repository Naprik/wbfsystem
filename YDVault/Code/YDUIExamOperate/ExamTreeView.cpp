// ExamTreeView.cpp : 实现文件
//

#include "stdafx.h"
//#include "MainFrm.h"
//#include "YDVaultMain.h"
#include "ExamTreeView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "resource.h"
#include "../Include/YDExamComm.h"
#include "../YDUIUserManagement\AuthorityHelper.h"



// CExamTreeView

CExamTreeView::CExamTreeView()
{
	m_wndExamOperateView.SetTreeType(EXAMOPERATE_TREE);
}

CExamTreeView::~CExamTreeView()
{
}


BEGIN_MESSAGE_MAP(CExamTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_MESSAGE(WM_YD_TREE_SEL_CHANGE,OnTreeItemChanged)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CExamTreeView 消息处理程序



int CExamTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndExamOperateView.Create(dwViewStyle, rectDummy, this, IDC_EXAM_TREE))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	HRESULT hr = E_FAIL;
	hr = CreateTreeIcon();
	if(FAILED(hr))
	{
		TRACE0("Failed to CreateTreeIcon\n");
		return -1;
	}
	
	OnChangeVisualStyle();

	FillExamView();

	return 0;
}

HRESULT	CExamTreeView::CreateTreeIcon()
{
	HRESULT hr = E_FAIL;
	m_ImageSmall.Create (16, 16, ILC_COLOR16|ILC_MASK, 2, 1);
	m_ImageLarge.Create (32, 32, ILC_COLOR16|ILC_MASK, 2, 1);
	for (int i = 0; i < 20; ++i)
	{
		HICON hIcon;
		switch(i)
		{
		case 1:
			hIcon = AfxGetApp()->LoadIcon(IDI_LINK);
			break;
		case 2:
			hIcon = AfxGetApp()->LoadIcon(IDI_RADIO);
			break;
		case 3:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_ALL);
			break;
		case 4:
			hIcon = AfxGetApp()->LoadIcon(IDI_SINGLE);
			break;
		case 5:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROOLING);
			break;
		case 6:
			hIcon = AfxGetApp()->LoadIcon(IDI_SPESH);
			break;
		case 7:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_SINGLE);
			break;
		case 8:
			hIcon = AfxGetApp()->LoadIcon(IDI_ROLL_SINGLE);
			break;
		case 9:
			hIcon = AfxGetApp()->LoadIcon(IDI_IMPORT);
			break;
		case 10:
			hIcon = AfxGetApp()->LoadIcon(IDI_DEPART);
			break;
		case 11:
			hIcon = AfxGetApp()->LoadIcon(IDI_PP);
			break;
		case 12:
			hIcon = AfxGetApp()->LoadIcon(IDI_ANSWER);
			break;
		case 13:
			hIcon = AfxGetApp()->LoadIcon(IDI_TEACH);
			break;			
		case 14:
			hIcon = AfxGetApp()->LoadIcon(IDI_PAPER);
			break;		
		case 15:
			hIcon = AfxGetApp()->LoadIcon(IDI_SUB);
			break;		
		case 16:
			hIcon = AfxGetApp()->LoadIcon(IDI_WEB);
			break;
		case 17:
			hIcon = AfxGetApp()->LoadIcon(IDI_WEB);
			break;

		default:
			hIcon = AfxGetApp()->LoadIcon(IDI_RADIO);
			break;
		}
		ASSERT(hIcon);
		m_ImageSmall.Add(hIcon);
		m_ImageLarge.Add(hIcon);
	}
	m_wndExamOperateView.SetImageList(&m_ImageSmall,TVSIL_NORMAL);
	return S_OK;
}
void CExamTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rectTree;
	m_wndExamOperateView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CExamTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndExamOperateView.SetFocus();
}

void CExamTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CExamTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndExamOperateView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top/* + cyTlb*/ + 1, rectClient.Width() - 2, rectClient.Height() /*- cyTlb*/ - 2, SWP_NOACTIVATE | SWP_NOZORDER);	
}

void CExamTreeView::FillExamView()
{
	//ASSERT(theApp.m_pDatabase != NULL);

	HRESULT hr = E_FAIL;

	//生成根节点
	HTREEITEM hRootItem =  m_wndExamOperateView.InsertItem(EXAM_EXAM_OPERATE, NULL, NULL);//考试操作
	{
		HTREEITEM hRollCallItem = m_wndExamOperateView.InsertItem(EXAM_EXAM,0,0,hRootItem);
		//m_wndExamOperateView.InsertItem(EXAM_EXAM,0,0,hRollCallItem);//考试
		VARIANT_BOOL bBlueToothRight = VARIANT_FALSE;
		CAuthorityHelper helper;
		hr = helper.CheckSecurity(SECURITY_BLUE_TOOTH,&bBlueToothRight);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONQUESTION);
			return;
		}
		if(bBlueToothRight)
		{
			m_wndExamOperateView.InsertItem(EXAM_BLUE,0,0,hRollCallItem);//蓝牙考试
			m_wndExamOperateView.InsertItem(EXAM_EXTACT_ANSWER,0,0,hRollCallItem);
		}
		VARIANT_BOOL bRedOutRight = VARIANT_FALSE;
		hr = helper.CheckSecurity(SECURITY_REDOUT,&bRedOutRight);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONQUESTION);
			return;
		}
		if(bRedOutRight)
		{
			m_wndExamOperateView.InsertItem(EXAN_FETCH_ANSWER,0,0,hRollCallItem);//红外提取答案
			m_wndExamOperateView.InsertItem(EXAM_INFRARED,0,0,hRollCallItem);
		}
		m_wndExamOperateView.InsertItem(EXAM_MARK_INFO,0,0,hRootItem);
		m_wndExamOperateView.InsertItem(EXAM_PAPER_ANSYS,0,0,hRootItem);
		m_wndExamOperateView.InsertItem(EXAM_ERR_REPORT,0,0,hRootItem);
		m_wndExamOperateView.Expand(hRollCallItem,TVE_EXPAND);
	}
	m_wndExamOperateView.Expand(hRootItem, TVE_EXPAND);
}

void CExamTreeView::OnChangeVisualStyle()
{
// 	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;
// 
// 	CBitmap bmp;
// 	if (!bmp.LoadBitmap(uiBmpId))
// 	{
// 		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
// 		ASSERT(FALSE);
// 		return;
// 	}
// 
// 	BITMAP bmpObj;
// 	bmp.GetBitmap(&bmpObj);
// 
// 	UINT nFlags = ILC_MASK;
// 
// 	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
// 
// 	m_ExamViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
// 	m_ExamViewImages.Add(&bmp, RGB(255, 0, 0));
// 
// 	m_wndExamView.SetImageList(&m_ExamViewImages, TVSIL_NORMAL);
}

HRESULT CExamTreeView::OnTreeItemChanged(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	HTREEITEM hItem = (HTREEITEM)wParam;
	ASSERT(hItem != NULL);
	if(hItem == m_wndExamOperateView.GetRootItem())
	{
		return S_OK;
	}
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_OPERATE,&bRight);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有执行此操作的权限"));
		return S_FALSE;
	}

	
	CString test;
	if (m_wndExamOperateView.ItemHasChildren(hItem))
	{
		return S_OK;
	}
	else
	{
		test = m_wndExamOperateView.GetItemText(hItem);
	}
	if(test.CompareNoCase(EXAM_EXAM) == 0)
	{
		return S_OK;
	}
	//蓝牙和红外的权限要特殊判断
	if(test.CompareNoCase(EXAM_BLUE) == 0||
		test.CompareNoCase(EXAM_EXTACT_ANSWER) == 0)
	{
		VARIANT_BOOL bBlueToothRight = VARIANT_FALSE;
		hr = helper.CheckSecurity(SECURITY_BLUE_TOOTH,&bBlueToothRight);
		if(FAILED(hr))
		{
			return hr;
		}
		if(!bBlueToothRight)
		{
			AfxMessageBox(_T("当前用户没有执行此操作的权限"));
			return S_FALSE;
		}

	}
	else if(test.CompareNoCase(EXAN_FETCH_ANSWER) == 0||
		test.CompareNoCase(EXAM_INFRARED) == 0)
	{
		VARIANT_BOOL bRedOutToothRight = VARIANT_FALSE;
		hr = helper.CheckSecurity(SECURITY_REDOUT,&bRedOutToothRight);
		if(FAILED(hr))
		{
			return hr;
		}
		if(!bRedOutToothRight)
		{
			AfxMessageBox(_T("当前用户没有执行此操作的权限"));
			return S_FALSE;
		}

	}
	COpenObjExParam OpenObjExParam;
	OpenObjExParam.m_strName = test;
	OpenObjExParam.m_Op = OP_VIEW;
	OpenObjExParam.m_bUseStaicStruct = FALSE;
	hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT_EX,(WPARAM)(&OpenObjExParam),0);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return hr;
	}
	return S_OK;	
}

void CExamTreeView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
