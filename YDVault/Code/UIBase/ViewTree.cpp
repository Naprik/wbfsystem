
#include "stdafx.h"
#include "ViewTree.h"
#include "../Include/YDVaultComm.h"
#include "../YDUIUserManagement\AuthorityHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
	m_bTriggerSelChange = TRUE;
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if(!m_bTriggerSelChange)
	{
		return;
	}
	HRESULT hr = E_FAIL;
	if(pNMTreeView->itemNew.hItem != NULL)
	{
		GetParent()->SendMessage(WM_YD_TREE_SEL_CHANGE,(WPARAM)pNMTreeView->itemNew.hItem,0);
// 		if (m_treeType == QUESTION_TREE)
// 		{
// 			GetParent()->SendMessage(WM_YD_TREE_SEL_CHANGE,(WPARAM)pNMTreeView->itemNew.hItem,0);
// 		}
// 		else if (m_treeType == PAPER_TREE)
// 		{
// 			GetParent()->SendMessage(WM_YD_PFTREE_SEL_CHANGE, (WPARAM)pNMTreeView->itemNew.hItem, 0);
// 		}
// 		else if (m_treeType == EXAM_TREE)
// 		{
// 			GetParent()->SendMessage(WM_YD_EXAM_SEL_CHANGE, (WPARAM)pNMTreeView->itemNew.hItem, 0);
// 		}
			
		
	}
}

void CViewTree::SetTreeType(TREE_TYPE type)
{
	m_treeType = type;
}
void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hItem = GetSelectedItem();
	if(hItem)
	{
		GetParent()->SendMessage(WM_YD_TREE_SEL_CHANGE,(WPARAM)hItem,0);
	}
	
}
