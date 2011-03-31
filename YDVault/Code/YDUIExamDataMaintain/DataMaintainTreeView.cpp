#include "StdAfx.h"
#include "DataMaintainTreeView.h"
#include "resource.h"
#include "../YDUIUserManagement\AuthorityHelper.h"

CDataMaintainTreeView::CDataMaintainTreeView(void)
{
	m_wndDataMaintainTreeView.SetTreeType(DATAMAINTAIN_TREE);
}

CDataMaintainTreeView::~CDataMaintainTreeView(void)
{
}

BEGIN_MESSAGE_MAP(CDataMaintainTreeView, CDockablePane)
 	ON_WM_CREATE()
 	ON_WM_SIZE()
 	ON_WM_PAINT()
 	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
 	ON_MESSAGE(WM_YD_TREE_SEL_CHANGE,OnTreeItemChanged)
END_MESSAGE_MAP()

void CDataMaintainTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	m_wndDataMaintainTreeView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top /*+ cyTlb*/ + 1, rectClient.Width() - 2, rectClient.Height() /*- cyTlb */- 2, SWP_NOACTIVATE | SWP_NOZORDER);
}


void	CDataMaintainTreeView::FillDataView()
{
	HRESULT hr = E_FAIL;

	//生成根节点
	HTREEITEM hItem =  m_wndDataMaintainTreeView.InsertItem(EXAM_DATAMGR_TREE, NULL, NULL);

	HTREEITEM hFmTtem = m_wndDataMaintainTreeView.InsertItem(EXAM_DEPT_INFO,0,0,hItem);
	m_wndDataMaintainTreeView.InsertItem(EXAM_STU_INFO,0,0,hItem);
	m_wndDataMaintainTreeView.InsertItem(EXAM_STU_DEVICE,0,0,hItem);
	m_wndDataMaintainTreeView.InsertItem(EXAM_TEACHER_DEVICE_INFO,0,0,hItem);
	m_wndDataMaintainTreeView.InsertItem(EXAM_PAPER_INFO,0,0,hItem);
	m_wndDataMaintainTreeView.InsertItem(EXAM_SUBJECT_INFO,0,0,hItem);
	m_wndDataMaintainTreeView.InsertItem(EXAM_EXAMADDR_INFO,0,0,hItem);
	

	m_wndDataMaintainTreeView.Expand(hItem, TVE_EXPAND);
}

int CDataMaintainTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndDataMaintainTreeView.Create(dwViewStyle, rectDummy, this, IDC_DATA_MAINTAIN_TREE))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	OnChangeVisualStyle();

	FillDataView();

	return 0;
}

void CDataMaintainTreeView::OnChangeVisualStyle()
{

}

void CDataMaintainTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rectTree;
	m_wndDataMaintainTreeView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CDataMaintainTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndDataMaintainTreeView.SetFocus();
}

void CDataMaintainTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

HRESULT CDataMaintainTreeView::OnTreeItemChanged(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	HTREEITEM hItem = (HTREEITEM)wParam;
	ASSERT(hItem != NULL);
	if(hItem == m_wndDataMaintainTreeView.GetRootItem())
	{
		return S_OK;
	}
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_DATAMAINTAIN,&bRight);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有执行此操作的权限"));
		return S_FALSE;
	}
	CString strItemText =  m_wndDataMaintainTreeView.GetItemText(hItem);
	COpenObjExParam OpenObjExParam;
	OpenObjExParam.m_strName = strItemText;
	OpenObjExParam.m_Op = OP_VIEW;
	OpenObjExParam.m_bUseStaicStruct = FALSE;
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT_EX,(WPARAM)(&OpenObjExParam),0);
	return S_OK;

}

void CDataMaintainTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndDataMaintainTreeView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		
		return;
	}
}