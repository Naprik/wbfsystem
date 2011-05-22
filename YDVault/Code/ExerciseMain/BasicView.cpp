// PdemMainView.cpp : implementation of the CObjPropertyView class
//

#include "stdafx.h"
#include "BasicView.h"

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView

IMPLEMENT_DYNCREATE(CBasicView, CTabView)

BEGIN_MESSAGE_MAP(CBasicView, CTabView)
	//{{AFX_MSG_MAP(CObjPropertyView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
//	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView construction/destruction

CBasicView::CBasicView()
{
	m_nPrevActivePage = -1;
}

CBasicView::~CBasicView()
{
	
}

BOOL CBasicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView drawing

void CBasicView::OnDraw(CDC* pDC)
{
// 	CDocument* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView printing



LRESULT CBasicView::OnChangeActiveTab (WPARAM wp, LPARAM lp)
{
	CTabView::OnChangeActiveTab (wp,lp);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView diagnostics

#ifdef _DEBUG
void CBasicView::AssertValid() const
{
	CView::AssertValid();
}

void CBasicView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CExerciseMainDoc* CBasicView::GetDocument() // non-debug version is inline
{
	return (CExerciseMainDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView message handlers

void CBasicView::OnContextMenu(CWnd*, CPoint point)
{
	//theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CBasicView::OnInitialUpdate() 
{
	CTabView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
 	m_wndTabs.EnableTabSwap(FALSE);

}

int CBasicView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	 //TODO: Add your specialized creation code here
	m_wndTabs.SetLocation(CMFCBaseTabCtrl::LOCATION_TOP);
	

	
	return 0;
}

void CBasicView::OnDestroy()
{
	
	CTabView::OnDestroy();
}
