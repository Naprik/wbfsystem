// PdemMainView.cpp : implementation of the CObjPropertyView class
//

#include "stdafx.h"
#include "ObjPropertyView.h"
#include "ObjPropertySheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView

IMPLEMENT_DYNCREATE(CObjPropertyView, CTabView)

BEGIN_MESSAGE_MAP(CObjPropertyView, CTabView)
	//{{AFX_MSG_MAP(CObjPropertyView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
//	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView construction/destruction

CObjPropertyView::CObjPropertyView()
{
	// TODO: add construction code here
	m_pSheet = NULL;
	m_nPrevActivePage = -1;
}

CObjPropertyView::~CObjPropertyView()
{
	if(m_pSheet != NULL)
	{
		delete m_pSheet;
		m_pSheet = NULL;
	}
}

BOOL CObjPropertyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView drawing

void CObjPropertyView::OnDraw(CDC* pDC)
{
// 	CDocument* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView printing

void CObjPropertyView::OnFilePrintPreview() 
{
	//BCGPPrintPreview (this);
}

BOOL CObjPropertyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CObjPropertyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CObjPropertyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

LRESULT CObjPropertyView::OnChangeActiveTab (WPARAM wp, LPARAM lp)
{
	CTabView::OnChangeActiveTab (wp,lp);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView diagnostics

#ifdef _DEBUG
void CObjPropertyView::AssertValid() const
{
	CView::AssertValid();
}

void CObjPropertyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CYDVaultMainDoc* CObjPropertyView::GetDocument() // non-debug version is inline
{
	return (CYDVaultMainDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjPropertyView message handlers

void CObjPropertyView::OnContextMenu(CWnd*, CPoint point)
{
	//theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}

void CObjPropertyView::OnInitialUpdate() 
{
	CTabView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
 	m_wndTabs.EnableTabSwap(FALSE);

	ResetObjWrapper(NULL);
}

HRESULT CObjPropertyView::ResetObjWrapper(CYdObjWrapper* pWrapper)
{
	return S_OK;
}

int CObjPropertyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	 //TODO: Add your specialized creation code here
	m_wndTabs.SetLocation(CMFCBaseTabCtrl::LOCATION_TOP);
	
// 	CRect rectDummy;
// 	rectDummy.SetRectEmpty ();
// 	
// 	// Create tabs window:
// 	if (!m_wndTabs.Create (
// 		IsScrollBar () ? 
// 		CBCGPTabWnd::STYLE_FLAT_SHARED_HORZ_SCROLL : CBCGPTabWnd::STYLE_FLAT, 
// 		rectDummy, this, 1,CBCGPTabWnd::LOCATION_TOP))
// 	{
// 		TRACE0("Failed to create tab window\n");
// 		return -1;      // fail to create
// 	}
// 	
// 	m_wndTabs.SetFlatFrame ();
// 	m_wndTabs.SetTabBorderSize (0);
// 	m_wndTabs.AutoDestroyWindow (FALSE);
	
	return 0;
}

void CObjPropertyView::OnDestroy()
{
	CObjPropertySheet* pSheet = GetPropertySheet();
// 	if(pSheet != NULL)
// 	{
// 		//CWnd* pMainfrm = CWnd::FromHandle(_ghwndMainFrm);
// 		CObjPropSheetManager* pSheetManager = (CObjPropSheetManager*)AfxGetMainWnd()->SendMessage(WM_GET_PROPSHEETMANAGER);
// 		HRESULT hr = pSheetManager->Remove(pSheet);
// 		if(SUCCEEDED(hr))
// 		{
// 		}
// 		else
// 		{
// 			DISPLAY_PDEMERROR(hr,MB_OK|MB_ICONSTOP);
// 			return;
// 		}
// 	}
	CTabView::OnDestroy();
}

HRESULT CObjPropertyView::CreateSheet(CYdObjWrapper* _pObjWrapper,OPERATION _nOp)
{
	ASSERT(m_pSheet == NULL);
	m_pSheet = new CObjPropertySheet(this);
	if(_pObjWrapper)
		m_pSheet->Show(_pObjWrapper,_nOp);
//	else
//		m_pSheet->Show(_T("FMÊÕÒô»ú"),_nOp);
	return S_OK;
}

HRESULT CObjPropertyView::CreateSheetEx(CString sTitle, OPERATION _nOp)
{
	ASSERT(m_pSheet == NULL);
	HRESULT hr = E_FAIL;
	m_pSheet = new CObjPropertySheet(this);
	hr = m_pSheet->Show(sTitle,_nOp);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}