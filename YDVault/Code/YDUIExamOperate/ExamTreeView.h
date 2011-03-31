#pragma once
#include "../UIBase/ViewTree.h"
class CYdObjWrapper;


class  CExamTreeViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class BASE_DLL_API CExamTreeView : public CDockablePane
{
	
public:
	CExamTreeView();
	virtual ~CExamTreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CViewTree					m_wndExamOperateView;
	CImageList					m_ExamViewImages;
	UINT						m_nCurrSort;

	void						FillExamView();
	HRESULT						CreateTreeIcon();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg	HRESULT OnTreeItemChanged(WPARAM wParam, LPARAM lParam);


private:
	std::list<CYdObjWrapper*> m_lstClear;//”√”⁄ Õ∑≈
	CImageList	   m_ImageLarge;
	CImageList	   m_ImageSmall;
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


