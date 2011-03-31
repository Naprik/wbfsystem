
#include "stdafx.h"
#include "mainfrm.h"
#include "QuestionTreeView.h"
#include "Resource.h"
#include "YDVaultMain.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDQuestionVaultClassification.h"
#include "../Base/DataHandler.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Include/ShowErr.h"
#include "../ObjRef/YDLinkRef.h"
#include "../UIBase\PdemWait.h"
#include "../ObjRef/YDQuestionType.h"
#include "../ObjRef/YDChoiceQuestionRef.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../\YDUIUserManagement\AuthorityHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuestionTreeView

CQuestionTreeView::CQuestionTreeView()
{
	m_wndQuestionView.SetTreeType(QUESTION_TREE);
}

CQuestionTreeView::~CQuestionTreeView()
{
	CListAutoClean<CYdObjWrapper> clr(m_lstClear);
}

BEGIN_MESSAGE_MAP(CQuestionTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()

	ON_MESSAGE(WM_YD_TREE_SEL_CHANGE,OnTreeItemChanged)
	ON_COMMAND(ID_QUESTION_VAULT_REFRESH, &CQuestionTreeView::OnQuestionVaultClassificationRefresh)
    ON_COMMAND(ID_QUESTION_VAULT_EDIT, &CQuestionTreeView::OnQuestionVaultClassificationEdit)
	ON_COMMAND(ID_QUESTION_VAULT_NEW_QUESTION_VAULT_CLASSIFIACTION, &CQuestionTreeView::OnQuestionVaultClassificationNew)
	ON_COMMAND(ID_QUESTION_VAULT_DEL, &CQuestionTreeView::OnQuestionVaultClassificationDel)
	ON_COMMAND(ID_QUESTION_VAULT_NEW_QUESTION_VAULT, &CQuestionTreeView::OnQuestionVaultNew)
	ON_COMMAND(ID_NEW_QUESTION, &CQuestionTreeView::OnQuestionNew)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CQuestionTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndQuestionView.Create(dwViewStyle, rectDummy, this, IDC_QUESTION_TREE_VIEW))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);

	CBitmap bmpQVC;
	bmpQVC.LoadBitmap(IDB_BITMAP_QUESTION_VAULT_CLASSIFICATION);	
	m_FileViewImages.Add(&bmpQVC, RGB(255,0,255));

	CBitmap bmpQV;
	bmpQV.LoadBitmap(IDB_BITMAP_QUESTION_VAULT);	
	m_FileViewImages.Add(&bmpQV, RGB(255,0,255));

	CBitmap bmpQType;
	bmpQType.LoadBitmap(IDB_BITMAP_QUESTION_TYPE);	
	m_FileViewImages.Add(&bmpQType, RGB(255,0,255));

	int i = m_FileViewImages.GetImageCount();

	

	m_wndQuestionView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

// 	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
// 
// 	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
// 
// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
// 
// 	m_wndToolBar.SetOwner(this);
// 
// 	// All commands will be routed via this control , not via the parent frame:
// 	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillQuestionView();
	AdjustLayout();

	return 0;
}

void CQuestionTreeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CQuestionTreeView::FillQuestionView()
{
	ASSERT(theApp.m_pDatabase != NULL);

	HRESULT hr = E_FAIL;
	
	//生成根节点
	CString strRootName;
	strRootName.LoadString(IDS_YDQVAULT_TREE_ROOT);
	HTREEITEM hItem = m_wndQuestionView.InsertItem(strRootName, NULL, NULL);
	CYDQuestionVaultClassification* pRootQvc = new CYDQuestionVaultClassification(theApp.m_pDatabase);
	hr = pRootQvc->SetID(0);//设根节点的对象ID为0
	if(FAILED(hr))
	{
		return ;
	}
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = pRootQvc;
	pObjWrapper->m_pRelatedWnd = &m_wndQuestionView;
	pObjWrapper->m_hRelatedItem = hItem;
	m_wndQuestionView.SetItemData(hItem,(DWORD_PTR)pObjWrapper);
	m_lstClear.push_back(pObjWrapper);

	hr = RefreshNode(hItem);
	if(FAILED(hr))
	{
		return ;
	}
	m_wndQuestionView.Expand(hItem, TVE_EXPAND);


}

HRESULT CQuestionTreeView::RefreshNode(HTREEITEM _hItem)//刷新_hItem数据
{
	HRESULT hr = E_FAIL;
	hr = DelAllChildNode(_hItem);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(theApp.m_pDatabase != NULL);
	
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(_hItem);
	ASSERT(pObjWrapper != NULL);
	CString strDBName;
	hr = pObjWrapper->m_pObjRef->GetDBName(strDBName);
	if(FAILED(hr))
	{
		return hr;
	}
	if( strDBName.CompareNoCase(DB_QVAULTCLASSIFICATION) == 0)
	{
		CYDQuestionVaultClassification* pQVC = (CYDQuestionVaultClassification*)pObjWrapper->m_pObjRef;
		ASSERT(pQVC != NULL);
		{
			//所有的字试题库大类
			std::list<CYDQuestionVaultClassification*> lstChild;
			std::list<CYDLinkRef*> lstLinkRef;
			hr = pQVC->GetChildQVClassification(&lstChild,&lstLinkRef);
			if(FAILED(hr))
			{
				return hr;
			}
			ASSERT(lstChild.size() == lstLinkRef.size());
			std::list<CYDQuestionVaultClassification*>::const_iterator itrChild = lstChild.begin();
			std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
			for(;itrChild != lstChild.end() && itrLink != lstLinkRef.end();++itrChild,++itrLink)
			{
				hr = InsertYDObject(_hItem,(*itrChild),(*itrLink));
				if(FAILED(hr))
				{
					return hr;
				}
			}
		}
		{
			//所有的字试题
			std::list<CYDQuestionVault*> lstChildQVault;
			std::list<CYDLinkRef*> lstLinkRef;
			hr = pQVC->GetAllQVault(&lstChildQVault,&lstLinkRef);
			if(FAILED(hr))
			{
				return hr;
			}
			ASSERT(lstChildQVault.size() == lstLinkRef.size());
			std::list<CYDQuestionVault*>::const_iterator itr = lstChildQVault.begin();
			std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
			for(;itr != lstChildQVault.end() && itrLink != lstLinkRef.end();++itr,++itrLink)
			{
				hr = InsertYDObject(_hItem,(*itr),(*itrLink));
				if(FAILED(hr))
				{
					return hr;
				}
			}

		}
	
	}
	else if( strDBName.CompareNoCase(DB_QUESTIONVAULT) == 0)
	{
		//题库
		CYDQuestionVault* pQV = (CYDQuestionVault*)pObjWrapper->m_pObjRef;
		ASSERT(pQV);
		std::list<CYDQuestionType*> lstChildQType;
		std::list<CYDLinkRef*> lstLinkRef;
		hr= pQV->GetAllQuestionType(&lstChildQType,&lstLinkRef);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstChildQType.size() == lstLinkRef.size());
		std::list<CYDQuestionType*>::const_iterator itr = lstChildQType.begin();
		std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
		for(;itr != lstChildQType.end() && itrLink != lstLinkRef.end();++itr,++itrLink)
		{
			CYDObjectRef* pObj = (CYDObjectRef*)(*itr);
			hr = InsertYDObject(_hItem,pObj,(*itrLink));
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}
	else if( strDBName.CompareNoCase(DB_QUESTIONTYPE) == 0)
	{
		//题型
		CYDQuestionType* pQType =  (CYDQuestionType*)pObjWrapper->m_pObjRef;
		ASSERT(pQType );
		UINT uTypeID = 0;
		hr = pQType->GetID(&uTypeID);
		if(FAILED(hr))
		{
			return hr;
		}

	}
	return S_OK;
}

HRESULT CQuestionTreeView::InsertYDObject(HTREEITEM _hParent,
										 CYDObjectRef* _pObj,
										 CYDLinkRef* _pLink /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	hr = _pObj->Restore();
	if (FAILED(hr))
	{
		return hr;
	}
	CString strDBName;
	hr = _pObj->GetDBName(strDBName);
	if (FAILED(hr))
	{
		return hr;
	}
	int iImageIndex = 0;
	if(strDBName.CompareNoCase(DB_QVAULTCLASSIFICATION) == 0)
	{
		iImageIndex = 0;
	}
	else if(strDBName.CompareNoCase(DB_QUESTIONVAULT) == 0)
	{
		iImageIndex = 1;
	}
	else if(strDBName.CompareNoCase(DB_QUESTIONTYPE) == 0)
	{
		iImageIndex = 2;
	}
	_variant_t valName;
	hr = _pObj->GetPropVal(L"NAME",&valName);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strName = CDataHandler::VariantToString(valName);

	HTREEITEM hItem = m_wndQuestionView.InsertItem(strName,iImageIndex,iImageIndex,_hParent);
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = _pObj;
	pObjWrapper->m_pLinkRef = _pLink;
	pObjWrapper->m_pRelatedWnd = &m_wndQuestionView;
	pObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pObjWrapper);
	m_wndQuestionView.SetItemData(hItem, (DWORD_PTR)pObjWrapper);
	
	return S_OK;
}


HRESULT CQuestionTreeView::DelAllChildNode(HTREEITEM _hItem)//删除_hItem的所有子节点
{
	HRESULT hr = E_FAIL;
	while(m_wndQuestionView.ItemHasChildren(_hItem))
	{
		HTREEITEM hTemp = m_wndQuestionView.GetChildItem(_hItem);
		m_wndQuestionView.DeleteItem(hTemp);
	}
	return S_OK;
}

void CQuestionTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndQuestionView;
	ASSERT_VALID(pWndTree);
	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}
	CYdObjWrapper* pObjWrapper = NULL;
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
			pObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hTreeItem);
			ASSERT(pObjWrapper);
		}
	}

	pWndTree->SetFocus();
	HRESULT hr = E_FAIL;
	if(pObjWrapper != NULL)
	{
		CString strDBName;
		hr = pObjWrapper->m_pObjRef->GetDBName(strDBName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(strDBName.CompareNoCase(DB_QVAULTCLASSIFICATION) == 0)
		{
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_QUESTION_VAULT_CLASSIFICATION, point.x, point.y, this, TRUE);
		}
		else if(strDBName.CompareNoCase(DB_QUESTIONVAULT) == 0)
		{
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_QUESTION_VAULT, point.x, point.y, this, TRUE);
		}
		else if(strDBName.CompareNoCase(DB_QUESTIONTYPE) == 0)
		{
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MENU_QUESTION_TYPE, point.x, point.y, this, TRUE);
		}
	}
	
}

void CQuestionTreeView::AdjustLayout()
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
	m_wndQuestionView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top /*+ cyTlb*/ + 1, rectClient.Width() - 2, rectClient.Height() /*- cyTlb */- 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CQuestionTreeView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CQuestionTreeView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CQuestionTreeView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CQuestionTreeView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CQuestionTreeView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CQuestionTreeView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CQuestionTreeView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CQuestionTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndQuestionView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CQuestionTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndQuestionView.SetFocus();
}

void CQuestionTreeView::OnChangeVisualStyle()
{
// 	m_wndToolBar.CleanUpLockedImages();
// 	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	//m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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

	//m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	//m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	//m_wndQuestionView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

HRESULT CQuestionTreeView::OnTreeItemChanged(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	HTREEITEM hItem = (HTREEITEM)wParam;
	ASSERT(hItem != NULL);
	if(hItem == m_wndQuestionView.GetRootItem())
	{
		return S_OK;
	}
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_VAULT,&bRight);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bRight)
	{
		AfxMessageBox(_T("当前用户没有执行此操作的权限"));
		return S_FALSE;
	}

	
	if(!m_wndQuestionView.ItemHasChildren(hItem))
	{
		RefreshNode(hItem);
	}

	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hItem);
	ASSERT(pObjWrapper);
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,(WPARAM)pObjWrapper,OP_VIEW);
	
	return S_OK;
}

void CQuestionTreeView::OnQuestionVaultClassificationRefresh()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = m_wndQuestionView.GetSelectedItem();
	ASSERT(hItem);
	if(hItem == NULL)
	{
		return ;
	}
	AfxGetMainWnd()->SendMessage(WM_YD_CLOSE_ALL_PROP,0,0);
	RefreshNode(hItem);
	m_wndQuestionView.Expand(hItem,TVE_EXPAND);
}

void CQuestionTreeView::OnQuestionVaultClassificationEdit()
{
	HRESULT hr = E_FAIL;
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_VAULT,&bRight);
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
	HTREEITEM hItem = m_wndQuestionView.GetSelectedItem();
	ASSERT(hItem);
	if(hItem == NULL)
	{
		return ;
	}
	if(hItem == m_wndQuestionView.GetRootItem())
	{
		//根节点不能编辑属性
		if(hItem == m_wndQuestionView.GetRootItem())
		{
			AfxMessageBox(_T("根节点不能编辑属性！"),MB_OK|MB_ICONINFORMATION);
			return ;
		}
		return;
	}
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hItem);
	ASSERT(pObjWrapper);
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,WPARAM(pObjWrapper),OP_EDIT);
}

void CQuestionTreeView::OnQuestionVaultClassificationNew()
{
	HRESULT hr = E_FAIL;
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_VAULT,&bRight);
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
	HTREEITEM hParentItem = m_wndQuestionView.GetSelectedItem();
	ASSERT(hParentItem);
	if(hParentItem == NULL)
	{
		return ;
	}
	CYdObjWrapper* pParentObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hParentItem);
	ASSERT(pParentObjWrapper);
	CYDObjectRef* pParentObjRef = pParentObjWrapper->m_pObjRef;
	ASSERT(pParentObjRef);
	CYDQuestionVaultClassification* pNewYqc = new CYDQuestionVaultClassification(theApp.m_pDatabase);
	ASSERT(pNewYqc);
	CYDLinkRef* pLinkRef = new CYDLinkRef(theApp.m_pDatabase,DB_QVAULTCLASSIFICATIONHIERARCHY);
	hr = pLinkRef->PutObjRef(pParentObjRef,pNewYqc);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strLabel = _T("新建题库类");
	BOOL bOldTreeTriggerChange = FALSE;
	hr = m_wndQuestionView.GetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = m_wndQuestionView.SetTriggerSelChange(FALSE);
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
	tvItem.lParam = (long)pNewYqc;
	tvItem.iImage = 0;
	tvItem.iSelectedImage = 0;
	tvIns.item = tvItem;
	if(hParentItem == NULL)
		tvIns.hInsertAfter = TVI_ROOT;
	else
		tvIns.hInsertAfter = NULL;
	tvIns.hParent = hParentItem;
	HTREEITEM hItem = m_wndQuestionView.InsertItem(&tvIns);
	strLabel.ReleaseBuffer();
	CYdObjWrapper* pNewObjWrapper = new CYdObjWrapper();
	pNewObjWrapper->m_pObjRef = pNewYqc;
	pNewObjWrapper->m_pLinkRef = pLinkRef;
	pNewObjWrapper->m_pRelatedWnd = &m_wndQuestionView;
	pNewObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pNewObjWrapper);
	m_wndQuestionView.SelectItem(hItem);
	m_wndQuestionView.SetItemData(hItem,(DWORD)pNewObjWrapper);
	hr = m_wndQuestionView.SetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		return;
	}
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,WPARAM(pNewObjWrapper),OP_NEW);
}

void CQuestionTreeView::OnQuestionVaultNew()
{
	HRESULT hr = E_FAIL;
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_VAULT,&bRight);
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
	HTREEITEM hParentItem = m_wndQuestionView.GetSelectedItem();
	ASSERT(hParentItem);
	if(hParentItem == NULL)
	{
		return ;
	}
	CYdObjWrapper* pParentObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hParentItem);
	ASSERT(pParentObjWrapper);
	CYDObjectRef* pParentObjRef = pParentObjWrapper->m_pObjRef;
	ASSERT(pParentObjRef);
	CYDQuestionVault* pNewYqv = new CYDQuestionVault(theApp.m_pDatabase);
	ASSERT(pNewYqv);
	CYDLinkRef* pLinkRef = new CYDLinkRef(theApp.m_pDatabase,DB_QCLASSIFICATIONQVAULT);
	hr = pLinkRef->PutObjRef(pParentObjRef,pNewYqv);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strLabel = _T("新建题库");
	BOOL bOldTreeTriggerChange = FALSE;
	hr = m_wndQuestionView.GetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = m_wndQuestionView.SetTriggerSelChange(FALSE);
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
	tvItem.lParam = (long)pNewYqv;
	tvItem.iImage = 1;
	tvItem.iSelectedImage = 1;
	tvIns.item = tvItem;
	if(hParentItem == NULL)
		tvIns.hInsertAfter = TVI_ROOT;
	else
		tvIns.hInsertAfter = NULL;
	tvIns.hParent = hParentItem;
	HTREEITEM hItem = m_wndQuestionView.InsertItem(&tvIns);
	strLabel.ReleaseBuffer();
	CYdObjWrapper* pNewObjWrapper = new CYdObjWrapper();
	pNewObjWrapper->m_pObjRef = pNewYqv;
	pNewObjWrapper->m_pLinkRef = pLinkRef;
	pNewObjWrapper->m_pRelatedWnd = &m_wndQuestionView;
	pNewObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pNewObjWrapper);
	m_wndQuestionView.SelectItem(hItem);
	m_wndQuestionView.SetItemData(hItem,(DWORD)pNewObjWrapper);
	hr = m_wndQuestionView.SetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		return;
	}
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,WPARAM(pNewObjWrapper),OP_NEW);
}

void CQuestionTreeView::OnQuestionNew()
{
	HRESULT hr = E_FAIL;
	//判断权限
	VARIANT_BOOL bRight = VARIANT_FALSE;
	CAuthorityHelper helper;
	hr = helper.CheckSecurity(SECURITY_VAULT,&bRight);
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
	HTREEITEM hParentItem = m_wndQuestionView.GetSelectedItem();
	ASSERT(hParentItem);
	if(hParentItem == NULL)
	{
		return ;
	}
	CYdObjWrapper* pParentObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hParentItem);
	ASSERT(pParentObjWrapper);
	CYDObjectRef* pParentObjRef = pParentObjWrapper->m_pObjRef;
	ASSERT(pParentObjRef);
	ASSERT(pParentObjRef->IsKindOf(RUNTIME_CLASS(CYDQuestionType)));
	CYDQuestionType* pParentQType = (CYDQuestionType*)pParentObjRef;
	ASSERT(pParentQType);

	CYDObjectRef* pObjA = NULL;
	CYDObjectRef* pObjB = NULL;
	hr = pParentObjWrapper->m_pLinkRef->GetObjRef(pObjA,pObjB);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	ASSERT(pObjA != NULL);
	ASSERT(pObjA->IsKindOf(RUNTIME_CLASS(CYDQuestionVault)));
	CYDQuestionVault* pQV = (CYDQuestionVault*)pObjA;

	CComVariant valType;
	hr = pParentQType->GetPropVal(L"Type",&valType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	UINT uType = (UINT)CDataHandler::VariantToLong(valType);
	CYDQuestionRef* pQuestionRef = NULL;
	if(uType == 0)
	{
		//使用选择题界面
		pQuestionRef = new CYDChoiceQuestionRef(theApp.m_pDatabase);
	}
	else if(uType == 1)
	{
		//使用阅读理解题界面
		pQuestionRef = new CYDArticleQuestionRef(theApp.m_pDatabase);
	}
	else
	{
		ASSERT(FALSE);
	}
	ASSERT(pQuestionRef );

	UINT idQType = 0;//题型ID
	hr = pParentQType->GetID(&idQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CComVariant valQTypeID((long)idQType);
	hr = pQuestionRef->SetPropVal(L"TYPEID",&valQTypeID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	CYDLinkRef* pLinkRef = new CYDLinkRef(theApp.m_pDatabase,DB_VAULTQUESTION);
	hr = pLinkRef->PutObjRef(pQV,pQuestionRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = pLinkRef->AddPropDef(_T("ID_TYPE"),VT_UINT);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	UINT uQTypeID = 0;
	hr = pParentQType->GetID(&uQTypeID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CComVariant valQuestionTypID((long)uQTypeID);
	hr = pLinkRef->SetPropVal(L"ID_TYPE",&valQuestionTypID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strLabel = _T("新建试题");
	BOOL bOldTreeTriggerChange = FALSE;
	hr = m_wndQuestionView.GetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = m_wndQuestionView.SetTriggerSelChange(FALSE);
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
	tvItem.lParam = (long)pQuestionRef;
	tvItem.iImage = 1;
	tvItem.iSelectedImage = 1;
	tvIns.item = tvItem;
	if(hParentItem == NULL)
		tvIns.hInsertAfter = TVI_ROOT;
	else
		tvIns.hInsertAfter = NULL;
	tvIns.hParent = hParentItem;
	HTREEITEM hItem = m_wndQuestionView.InsertItem(&tvIns);
	strLabel.ReleaseBuffer();
	CYdObjWrapper* pNewObjWrapper = new CYdObjWrapper();
	pNewObjWrapper->m_pObjRef = pQuestionRef;
	pNewObjWrapper->m_pLinkRef = pLinkRef;
	pNewObjWrapper->m_pRelatedWnd = &m_wndQuestionView;
	pNewObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pNewObjWrapper);
	m_wndQuestionView.SelectItem(hItem);
	m_wndQuestionView.SetItemData(hItem,(DWORD)pNewObjWrapper);
	hr = m_wndQuestionView.SetTriggerSelChange(bOldTreeTriggerChange);
	if(FAILED(hr))
	{
		return;
	}
	AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT,WPARAM(pNewObjWrapper),OP_NEW);
}


void CQuestionTreeView::OnQuestionVaultClassificationDel()
{
	HRESULT hr = E_FAIL;
	HTREEITEM hItem = m_wndQuestionView.GetSelectedItem();
	ASSERT(hItem);
	if(hItem == NULL)
	{
		return ;
	}
	if(hItem == m_wndQuestionView.GetRootItem())
	{
		AfxMessageBox(_T("根节点不能删除！"),MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(AfxMessageBox(_T("你确定要删除吗？"),MB_YESNO) != IDYES)
	{
		return ;
	}
	CPdemWait	wait(_T("正在删除..."));
	wait.BeginWait();

	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_wndQuestionView.GetItemData(hItem);
	ASSERT(pObjWrapper);
	AfxGetMainWnd()->SendMessage(WM_YD_CLOSE_PROP,(WPARAM)pObjWrapper);
	hr = pObjWrapper->Remove();
	if(FAILED(hr))
	{
		return;
	}
	m_wndQuestionView.DeleteItem(hItem);
	
}