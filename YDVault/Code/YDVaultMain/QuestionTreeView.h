
#pragma once

#include "../UIBase/ViewTree.h"
#include <list>

class CYDObjectRef;
class CYDQuestionVaultClassification;
class CYdObjWrapper;
class CYDLinkRef;

class CQuestionTreeViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CQuestionTreeView : public CDockablePane
{
// Construction
public:
	CQuestionTreeView();
	virtual ~CQuestionTreeView();
	
public:
	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	void FillQuestionView();

public:
	afx_msg void OnQuestionVaultClassificationRefresh();
	afx_msg void OnQuestionVaultClassificationEdit();
	afx_msg void OnQuestionVaultClassificationNew();
	afx_msg void OnQuestionVaultNew();
	afx_msg void OnQuestionVaultClassificationDel();
	afx_msg void OnQuestionNew();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg	HRESULT OnTreeItemChanged(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
    HRESULT RefreshNode(HTREEITEM _hItem);//刷新_hItem数据
	HRESULT DelAllChildNode(HTREEITEM _hItem);//删除_hItem的所有子节点
	HRESULT InsertYDObject(HTREEITEM _hParent, 
								CYDObjectRef* _pObj,
								CYDLinkRef* _pLink = NULL);

protected:
	CViewTree					m_wndQuestionView;
	CImageList					m_FileViewImages;
//	CQuestionTreeViewToolBar	m_wndToolBar;


private:
	std::list<CYdObjWrapper*> m_lstClear;//用于释放
};

