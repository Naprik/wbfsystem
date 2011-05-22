
#pragma once


class CExerciseMainDoc;

class  CBasicView : public CTabView
{
protected: // create from serialization only
	CBasicView();
	DECLARE_DYNCREATE(CBasicView)

// Attributes
protected:

	int                      m_nPrevActivePage;
public:
	CExerciseMainDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjPropertyView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBasicView();
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
	afx_msg LRESULT OnChangeActiveTab (WPARAM wp, LPARAM);
	DECLARE_MESSAGE_MAP()
};
