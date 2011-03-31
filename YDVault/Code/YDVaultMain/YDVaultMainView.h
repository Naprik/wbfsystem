
// YDVaultMainView.h : interface of the CYDVaultMainView class
//


#pragma once


class CYDVaultMainView : public CView
{
protected: // create from serialization only
	CYDVaultMainView();
	DECLARE_DYNCREATE(CYDVaultMainView)

// Attributes
public:
	CYDVaultMainDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CYDVaultMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	
};

#ifndef _DEBUG  // debug version in YDVaultMainView.cpp
inline CYDVaultMainDoc* CYDVaultMainView::GetDocument() const
   { return reinterpret_cast<CYDVaultMainDoc*>(m_pDocument); }
#endif

