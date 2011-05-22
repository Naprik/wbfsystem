// ExamTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "PersionInfoTreeView.h"
#include "resource.h"
#include "ExerciseMain.h"
#include "../YDUIUserManagement/StaticYdUser.h"
// CExamTreeView

CPersionInfoTreeView::CPersionInfoTreeView()
{
}

CPersionInfoTreeView::~CPersionInfoTreeView()
{
}


BEGIN_MESSAGE_MAP(CPersionInfoTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_MESSAGE(WM_YD_TREE_SEL_CHANGE,OnTreeItemChanged)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CExamTreeView 消息处理程序



int CPersionInfoTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_SHOWSELALWAYS |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;

	if (!m_wndPersionInfoTreeView.Create(dwViewStyle, rectDummy, this, IDC_TREE))
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
	

	FillPersionInfoTreeView();

	return 0;
}

HRESULT	CPersionInfoTreeView::CreateTreeIcon()
{
	HRESULT hr = E_FAIL;
	CBitmap bmp;
	if (!bmp.LoadBitmap(IDB_BITMAP_MAINTREE))
	{
		return FALSE;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= ILC_COLOR32;

	m_PersionInfoViewImages.Create(32, bmpObj.bmHeight, nFlags, 0, 0);
	m_PersionInfoViewImages.Add(&bmp, RGB(0, 0, 0));

	m_wndPersionInfoTreeView.SetImageList(&m_PersionInfoViewImages, TVSIL_NORMAL);
	return S_OK;
}
void CPersionInfoTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rectTree;
	m_wndPersionInfoTreeView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CPersionInfoTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndPersionInfoTreeView.SetFocus();
}

void CPersionInfoTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPersionInfoTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndPersionInfoTreeView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top/* + cyTlb*/ + 1, rectClient.Width() - 2, rectClient.Height() /*- cyTlb*/ - 2, SWP_NOACTIVATE | SWP_NOZORDER);	
}

void CPersionInfoTreeView::FillPersionInfoTreeView()
{
	HRESULT hr = E_FAIL;

	//生成根节点
	HTREEITEM hRootItem =  m_wndPersionInfoTreeView.InsertItem(TREE_NODE_USER_INFO, 0, 0);
	
	HTREEITEM hRootItem2 =  m_wndPersionInfoTreeView.InsertItem(TREE_NODE_HISTORY, 1, 1);
}



HRESULT CPersionInfoTreeView::OnTreeItemChanged(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	if (theApp.m_bInit)
	{
		HTREEITEM hItem = (HTREEITEM)wParam;
		ASSERT(hItem != NULL);
		CString strText = m_wndPersionInfoTreeView.GetItemText(hItem);
		COpenObjExParam OpenObjExParam;
		OpenObjExParam.m_strName = strText;
		OpenObjExParam.m_Op = OP_VIEW;
		OpenObjExParam.m_bUseStaicStruct = FALSE;
		hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT_EX,(WPARAM)(&OpenObjExParam),0);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return hr;
		}
	}

	return S_OK;	
}

void CPersionInfoTreeView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
