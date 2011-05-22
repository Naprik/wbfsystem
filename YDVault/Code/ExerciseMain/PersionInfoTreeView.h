#pragma once
#include "../UIBase/ViewTree.h"

class  CPersionInfoTreeView : public CDockablePane
{
	
public:
	CPersionInfoTreeView();
	virtual ~CPersionInfoTreeView();

	void AdjustLayout();

protected:
	CViewTree					m_wndPersionInfoTreeView;
	CImageList					m_PersionInfoViewImages;
	UINT						m_nCurrSort;

	void						FillPersionInfoTreeView();
	HRESULT						CreateTreeIcon();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg	HRESULT OnTreeItemChanged(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


