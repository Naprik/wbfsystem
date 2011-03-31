
// YDVaultMainView.cpp : implementation of the CYDVaultMainView class
//

#include "stdafx.h"
#include "YDVaultMain.h"

#include "YDVaultMainDoc.h"
#include "YDVaultMainView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYDVaultMainView

IMPLEMENT_DYNCREATE(CYDVaultMainView, CView)

BEGIN_MESSAGE_MAP(CYDVaultMainView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CYDVaultMainView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CYDVaultMainView construction/destruction

CYDVaultMainView::CYDVaultMainView()
{
	// TODO: add construction code here

}

CYDVaultMainView::~CYDVaultMainView()
{
}

BOOL CYDVaultMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CYDVaultMainView drawing

void CYDVaultMainView::OnDraw(CDC* /*pDC*/)
{
	CYDVaultMainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CYDVaultMainView printing


void CYDVaultMainView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CYDVaultMainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CYDVaultMainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CYDVaultMainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CYDVaultMainView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CYDVaultMainView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}


// CYDVaultMainView diagnostics

#ifdef _DEBUG
void CYDVaultMainView::AssertValid() const
{
	CView::AssertValid();
}

void CYDVaultMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CYDVaultMainDoc* CYDVaultMainView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYDVaultMainDoc)));
	return (CYDVaultMainDoc*)m_pDocument;
}
#endif //_DEBUG


// CYDVaultMainView message handlers


