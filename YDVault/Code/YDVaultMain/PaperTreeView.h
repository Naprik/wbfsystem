
#pragma once

#include "../UIBase/ViewTree.h"

class CPaperTreeViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPaperTreeView : public CDockablePane
{
public:
	CPaperTreeView();
	virtual ~CPaperTreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
//	CPaperTreeViewToolBar		m_wndToolBar;
	CViewTree					m_wndPaperMgrView;
	CImageList					m_PaperMgrViewImages;
	UINT						m_nCurrSort;

	void						FillPaperView();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
	afx_msg	HRESULT OnTreeItemChanged(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	HRESULT RefreshNode(HTREEITEM _hItem);
	HRESULT InsertYDObject(HTREEITEM _hParent,
							CYDObjectRef* _pObj,
							CYDLinkRef* _pLink = NULL);
	HRESULT DelAllChildNode(HTREEITEM _hItem);


private:
	std::list<CYdObjWrapper*> m_lstClear;//”√”⁄ Õ∑≈
public:
	afx_msg void OnMiPftreePfDelete();
	afx_msg void OnMiPftreePfEdit();
	afx_msg void OnMIRefresh();
	afx_msg void OnMiPftreeNewpf();
	afx_msg void OnMIPaperImport();
	afx_msg void OnMIPaperDelete();
};

