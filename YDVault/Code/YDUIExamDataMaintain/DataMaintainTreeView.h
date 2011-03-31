#pragma once

#include "../UIBase/ViewTree.h"
class CDataMaintainTreeViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class BASE_DLL_API CDataMaintainTreeView :public CDockablePane
{
public:
	CDataMaintainTreeView(void);
	~CDataMaintainTreeView(void);
	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	//	CPaperTreeViewToolBar		m_wndToolBar;
	CImageList					m_PaperMgrViewImages;
	UINT						m_nCurrSort;

	void						FillDataView();

	// Overrides

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnNewFolder();

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
	afx_msg	HRESULT OnTreeItemChanged(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	CViewTree			m_wndDataMaintainTreeView;
};
