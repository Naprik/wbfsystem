
#include "stdafx.h"
#include "../ObjRef/YDPaperFolder.h"
#include "../ObjRef/YdObjWrapper.h"
#include "MainFrm.h"
#include "PaperTreeView.h"
#include "Resource.h"
#include "YDVaultMain.h"
#include "../Base/FilePathHelper.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "../Include/ShowErr.h"
#include "../DBBase/DBTransactionRef.h"
#include "../UIBase/PdemWait.h"
#include "../YDUIUserManagement\AuthorityHelper.h"

class CPaperTreeViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CPaperTreeView;

	DECLARE_SERIAL(CPaperTreeViewMenuButton)

public:
	CPaperTreeViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CPaperTreeViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaperTreeView::CPaperTreeView()
{
	m_wndPaperMgrView.SetTreeType(PAPER_TREE);
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CPaperTreeView::~CPaperTreeView()
{
	CListAutoClean<CYdObjWrapper> autoClean(m_lstClear);
}

BEGIN_MESSAGE_MAP(CPaperTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	ON_COMMAND(ID_MI_PFTREE_PF_DELETE, &CPaperTreeView::OnMiPftreePfDelete)
	ON_COMMAND(ID_MI_PFTREE_PF_EDIT, &CPaperTreeView::OnMiPftreePfEdit)
	ON_COMMAND(ID_MI_PFTREE_PF_NEWSUBPF, &CPaperTreeView::OnMiPftreeNewpf)
	ON_COMMAND(ID_MI_PFTREE_PF_REFRESH, &CPaperTreeView::OnMIRefresh)
	ON_COMMAND(ID_MI_PFTREE_ROOT_NEWPF, &CPaperTreeView::OnMiPftreeNewpf)
	ON_COMMAND(ID_MI_PFTREE_ROOT_REFRESH, &CPaperTreeView::OnMIRefresh)
	ON_MESSAGE(WM_YD_TREE_SEL_CHANGE,OnTreeItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaperTreeView message handlers

int CPaperTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPaperMgrView.Create(dwViewStyle, rectDummy, this, IDC_PARPER_TREE_VIEW))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
// 	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
// 	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
// 
// 	m_wndToolBar.SetOwner(this);
// 
// 	// All commands will be routed via this control , not via the parent frame:
// 	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;

	//m_wndToolBar.ReplaceButton(ID_SORT_MENU, CPaperTreeViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

// 	CPaperTreeViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CPaperTreeViewMenuButton, m_wndToolBar.GetButton(0));
// 
// 	if (pButton != NULL)
// 	{
// 		pButton->m_bText = FALSE;
// 		pButton->m_bImage = TRUE;
// 		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
// 		pButton->SetMessageWnd(this);
// 	}

	// Fill in some static tree view data (dummy code, nothing mIagic here)
	FillPaperView();

	return 0;
}

void CPaperTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPaperTreeView::FillPaperView()
{
	ASSERT(theApp.m_pDatabase != NULL);

	HRESULT hr = E_FAIL;

	//生成根节点
	CString strRootName;
	strRootName.LoadString(IDS_YDVAULTMAIN_PAPERMGR_ROOT);
	HTREEITEM hItem =  m_wndPaperMgrView.InsertItem(strRootName, NULL, NULL);
	CYDPaperFolder* pRootPF = new CYDPaperFolder(theApp.m_pDatabase);
	hr = pRootPF->SetID(0);//设根节点的对象ID为0
	if(FAILED(hr))
	{
		return ;
	}
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = pRootPF;
	pObjWrapper->m_pRelatedWnd = &m_wndPaperMgrView;
	pObjWrapper->m_hRelatedItem = hItem;
	m_wndPaperMgrView.SetItemData(hItem, (DWORD_PTR)pObjWrapper);
	m_lstClear.push_back(pObjWrapper);

	hr = RefreshNode(hItem);
	if(FAILED(hr))
	{
		return ;
	}
	m_wndPaperMgrView.Expand(hItem, TVE_EXPAND);
}

void CPaperTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndPaperMgrView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
			pWndTree->SetFocus();
			HTREEITEM hRootItem = pWndTree->GetRootItem();
			CMenu* pSubMenu = NULL;
			CMenu menu;
			if (hRootItem == hTreeItem)
			{
				menu.LoadMenu(IDR_MENU_PFTREE_ROOT);
				pSubMenu = menu.GetSubMenu(0);
			}
			else
			{
				CYdObjWrapper* pWrapper = (CYdObjWrapper*)
					pWndTree->GetItemData(hTreeItem);
				ASSERT(pWrapper != NULL);
				CString strDBName;
				pWrapper->m_pObjRef->GetDBName(strDBName);
				if (strDBName.CompareNoCase(DB_PAPERFOLDER) == 0)
				{
					menu.LoadMenu(IDR_MENU_PFTREE_PF);
					pSubMenu = menu.GetSubMenu(0);
				}

			}
			if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
			{
				CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

				if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSubMenu->m_hMenu, FALSE, TRUE))
					return;

				((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
				UpdateDialogControls(this, FALSE);
			}
		}
	}
}

void CPaperTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

// 	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
// 
// 	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPaperMgrView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top/* + cyTlb*/ + 1, rectClient.Width() - 2, rectClient.Height() /*- cyTlb*/ - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CPaperTreeView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CPaperTreeView::OnSort(UINT id)
{
// 	if (m_nCurrSort == id)
// 	{
// 		return;
// 	}
// 
// 	m_nCurrSort = id;
// 
// 	CPaperTreeViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CPaperTreeViewMenuButton, m_wndToolBar.GetButton(0));
// 
// 	if (pButton != NULL)
// 	{
// 		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
// 		m_wndToolBar.Invalidate();
// 		m_wndToolBar.UpdateWindow();
// 	}
}

void CPaperTreeView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}


void CPaperTreeView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CPaperTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndPaperMgrView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CPaperTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndPaperMgrView.SetFocus();
}

void CPaperTreeView::OnChangeVisualStyle()
{

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_PaperMgrViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_PaperMgrViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndPaperMgrView.SetImageList(&m_PaperMgrViewImages, TVSIL_NORMAL);

// 	m_wndToolBar.CleanUpLockedImages();
// 	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}


HRESULT CPaperTreeView::RefreshNode(HTREEITEM _hItem)//刷新_hItem数据
{
	HRESULT hr = E_FAIL;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndPaperMgrView.GetItemData(_hItem);
	ASSERT(pObjWrapper != NULL);

	CYDPaperFolder* pPF= dynamic_cast<CYDPaperFolder*>(pObjWrapper->m_pObjRef);

	if (pPF != NULL)
	{
		hr = DelAllChildNode(_hItem);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(theApp.m_pDatabase != NULL);
	
		std::list<CYDPaperFolder*> lstChild;
		std::list<CYDLinkRef*> lstLinkRef;
		hr = pPF->GetChildPaperFolder(&lstChild, &lstLinkRef);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstChild.size() == lstLinkRef.size());
		std::list<CYDPaperFolder*>::const_iterator itrChild = lstChild.begin();
		std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
		for(;itrChild != lstChild.end() && itrLink != lstLinkRef.end();++itrChild,++itrLink)
		{
			hr = InsertYDObject(_hItem, (*itrChild), (*itrLink));
			if(FAILED(hr))
			{
				return hr;
			}
		}

		
	}
	
	return S_OK;
}

HRESULT CPaperTreeView::InsertYDObject(HTREEITEM _hParent,
										  CYDObjectRef* _pObj,
										  CYDLinkRef* _pLink /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	hr = _pObj->Restore();
	if (FAILED(hr))
	{
		return hr;
	}
	_variant_t valName;
	hr = _pObj->GetPropVal(L"NAME", &valName);
	if(FAILED(hr))
	{
		return hr;
	}
	int nIndex = 2;
	if (_pObj->m_strDBName == DB_PAPER)
	{
		nIndex = 3;
	}
	CString strName = CDataHandler::VariantToString(valName);

	HTREEITEM hItem = m_wndPaperMgrView.InsertItem(strName,nIndex, nIndex,_hParent,NULL);
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = _pObj;
	pObjWrapper->m_pLinkRef = _pLink;
	pObjWrapper->m_pRelatedWnd = &m_wndPaperMgrView;
	pObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pObjWrapper);
	m_wndPaperMgrView.SetItemData(hItem, (DWORD_PTR)pObjWrapper);

	return S_OK;
}


HRESULT CPaperTreeView::DelAllChildNode(HTREEITEM _hItem)//删除_hItem的所有子节点
{
	HRESULT hr = E_FAIL;
	while(m_wndPaperMgrView.ItemHasChildren(_hItem))
	{
		HTREEITEM hTemp = m_wndPaperMgrView.GetChildItem(_hItem);
		m_wndPaperMgrView.DeleteItem(hTemp);
	}
	return S_OK;
}

void CPaperTreeView::OnMiPftreePfDelete()
{
	HTREEITEM hitem = m_wndPaperMgrView.GetSelectedItem();
	CYdObjWrapper* pWrapper = reinterpret_cast<CYdObjWrapper*>(m_wndPaperMgrView.GetItemData(hitem));
	ASSERT(pWrapper != NULL);
	CString strDBName;
	pWrapper->m_pObjRef->GetDBName(strDBName);
	CString strInfoFormat;
	strInfoFormat.LoadString(IDS_YDVAULTMAIN_DELETE_INFO_FORMAT);
	_variant_t varVal;
	HRESULT hr = pWrapper->m_pObjRef->GetPropVal(L"NAME", &varVal);
	if (FAILED(hr))
	{
		return;
	}
	CString strInfo;
	CString strName = CDataHandler::VariantToString(varVal);
	
	strInfo.Format(strInfoFormat, strName);
	if (strDBName == DB_PAPERFOLDER)
	{
		CString strWarning;
		strWarning.LoadString(IDS_YDVAULTMAIN_DELETEPF_WARNING);
		strInfo += strWarning;
	}
	if (IDOK == AfxMessageBox(strInfo, MB_OKCANCEL | MB_ICONQUESTION))
	{
		CPdemWait	wait(_T("正在删除..."));
		wait.BeginWait();

		CDBTransactionRef trans(theApp.m_pDatabase, TRUE);
		HRESULT hr = pWrapper->Remove();
		if (FAILED(hr))
		{
			trans.Rollback();
			return;
		}
		hr = trans.Commit();
		if (FAILED(hr))
		{
			trans.Rollback();
			return;
		}
		//删除树节点
		CTreeCtrl* pTree = reinterpret_cast<CTreeCtrl*>(&m_wndPaperMgrView);
		pTree->DeleteItem(hitem);
	}

}

void CPaperTreeView::OnMiPftreePfEdit()
{
	HRESULT hr = E_FAIL;
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_PAPER,&bRight);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONQUESTION);
		return;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有执行此操作的权限"));
		return;
	}
	HTREEITEM hItem = m_wndPaperMgrView.GetSelectedItem();
	ASSERT(hItem);
	if(hItem == NULL)
	{
		return ;
	}
	if(hItem == m_wndPaperMgrView.GetRootItem())
	{
		//根节点不能编辑属性
		if(hItem == m_wndPaperMgrView.GetRootItem())
		{
			AfxMessageBox(_T("根节点不能编辑属性！"),MB_OK|MB_ICONINFORMATION);
			return ;
		}
		return;
	}
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndPaperMgrView.GetItemData(hItem);
	ASSERT(pObjWrapper);
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,WPARAM(pObjWrapper),OP_EDIT);
}


void CPaperTreeView::OnMIRefresh()
{
	HTREEITEM hItem = m_wndPaperMgrView.GetSelectedItem();
	ASSERT(hItem);
	if(hItem == NULL)
	{
		return ;
	}
	AfxGetMainWnd()->SendMessage(WM_YD_CLOSE_ALL_PROP,0,0);
	RefreshNode(hItem);
	m_wndPaperMgrView.Expand(hItem,TVE_EXPAND);
}

void CPaperTreeView::OnMiPftreeNewpf()
{
	HRESULT hr = E_FAIL;
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_PAPER,&bRight);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONQUESTION);
		return;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有执行此操作的权限"));
		return;
	}
	HTREEITEM hParentItem = m_wndPaperMgrView.GetSelectedItem();
	ASSERT(hParentItem);
	if(hParentItem == NULL)
	{
		return ;
	}
	CYdObjWrapper* pParentObjWrapper = 
		(CYdObjWrapper*)m_wndPaperMgrView.GetItemData(hParentItem);
	ASSERT(pParentObjWrapper);
	CYDObjectRef* pParentObjRef = pParentObjWrapper->m_pObjRef;
	ASSERT(pParentObjRef);
	CYDPaperFolder* pNewPF = new CYDPaperFolder(theApp.m_pDatabase);
	ASSERT(pNewPF);
	CYDLinkRef* pLinkRef = new CYDLinkRef(theApp.m_pDatabase, DB_PFOLDERHIERARCHY);
	hr = pLinkRef->PutObjRef(pParentObjRef, pNewPF);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr, MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strLabel = _T("新建试卷夹");
	BOOL bOldTreeTriggerChange = FALSE;
	hr = m_wndPaperMgrView.GetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = m_wndPaperMgrView.SetTriggerSelChange(FALSE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	TV_ITEM tvItem;
	TV_INSERTSTRUCT tvIns;
	tvItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	int nLength = strLabel.GetLength() ;
	tvItem.pszText = strLabel.GetBuffer(nLength);
	tvItem.cchTextMax = nLength;
	tvItem.lParam = (long)pNewPF;
	tvItem.iImage = 0;
	tvItem.iSelectedImage = 0;
	tvIns.item = tvItem;
	if(hParentItem == NULL)
		tvIns.hInsertAfter = TVI_ROOT;
	else
		tvIns.hInsertAfter = NULL;
	tvIns.hParent = hParentItem;
	HTREEITEM hItem = m_wndPaperMgrView.InsertItem(&tvIns);
	strLabel.ReleaseBuffer();
	CYdObjWrapper* pNewObjWrapper = new CYdObjWrapper();
	pNewObjWrapper->m_pObjRef = pNewPF;
	pNewObjWrapper->m_pLinkRef = pLinkRef;
	pNewObjWrapper->m_pRelatedWnd = &m_wndPaperMgrView;
	pNewObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pNewObjWrapper);
	m_wndPaperMgrView.SelectItem(hItem);
	m_wndPaperMgrView.SetItemData(hItem,(DWORD)pNewObjWrapper);
	hr = m_wndPaperMgrView.SetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		return;
	}
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,WPARAM(pNewObjWrapper),OP_NEW);
}

HRESULT CPaperTreeView::OnTreeItemChanged(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	HTREEITEM hItem = (HTREEITEM)wParam;
	ASSERT(hItem != NULL);
	if(hItem == m_wndPaperMgrView.GetRootItem())
	{
		return S_OK;
	}
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_PAPER,&bRight);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONQUESTION);
		return hr;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有执行此操作的权限"));
		return S_FALSE;
	}

	
	if(!m_wndPaperMgrView.ItemHasChildren(hItem))
	{
		RefreshNode(hItem);
	}

	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndPaperMgrView.GetItemData(hItem);
	ASSERT(pObjWrapper);
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,(WPARAM)pObjWrapper,OP_VIEW);

	return S_OK;

}

