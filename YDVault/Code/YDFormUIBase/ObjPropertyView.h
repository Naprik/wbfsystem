// PdemMainView.h : interface of the CObjPropertyView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJPROPERTYVIEW_H__F927986F_24E1_4B42_B442_D1DDC13A30D6__INCLUDED_)
#define AFX_OBJPROPERTYVIEW_H__F927986F_24E1_4B42_B442_D1DDC13A30D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CObjPropertySheet;
class CYDVaultMainDoc;
class CYdObjWrapper;


class BASE_DLL_API CObjPropertyView : public CTabView
{
protected: // create from serialization only
	CObjPropertyView();
	DECLARE_DYNCREATE(CObjPropertyView)

// Attributes
protected:
	CObjPropertySheet*       m_pSheet;
	int                      m_nPrevActivePage;
public:
	CYDVaultMainDoc* GetDocument();

// Operations
public:
	void SetPropertySheet(CObjPropertySheet* pSheet){m_pSheet=pSheet;}
	CObjPropertySheet* GetPropertySheet(){return m_pSheet;}

	HRESULT ResetObjWrapper(CYdObjWrapper* pWrapper);
	HRESULT CreateSheet(CYdObjWrapper* _pObjWrapper,OPERATION _nOp);
	HRESULT CreateSheetEx(CString sTitle, OPERATION _nOp);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjPropertyView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObjPropertyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CObjPropertyView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnChangeActiveTab (WPARAM wp, LPARAM);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PdemMainView.cpp
inline CYDVaultMainDoc* CObjPropertyView::GetDocument()
   { return (CYDVaultMainDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJPROPERTYVIEW_H__F927986F_24E1_4B42_B442_D1DDC13A30D6__INCLUDED_)
