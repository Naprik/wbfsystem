
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once
#include "resource.h"
#include "BelowBar.h"

class CYdFormView;
class CYdObjWrapper;

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	CBelowBar m_belowbar;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	void ShowBelowBar(BOOL bShow){m_belowbar.ShowWindow(bShow);}
	void UpdateBelowBar(OPERATION type){m_belowbar.ShowBegin(type);}
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnPropertySave();
	afx_msg void OnPropertyUpdate();
	afx_msg void OnPropertyCancel();
	afx_msg void OnUpdatePropertySave(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertyUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertyCancel(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg HRESULT OnShowBelowBar(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnUpdateBelowBar(WPARAM wParam, LPARAM lParam);
private:
	HRESULT UpdateRelatedTree(CYdObjWrapper* _pObjWrapper);
public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
};
