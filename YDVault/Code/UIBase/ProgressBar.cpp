// ProgressBar.cpp : implementation file

	
#include "stdafx.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusControl

CStatusControl::CStatusControl()
{
}

CStatusControl::~CStatusControl()
{
}


BEGIN_MESSAGE_MAP(CStatusControl, CWnd)
	//{{AFX_MSG_MAP(CStatusControl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusControls message handlers

/****************************************************************************
*                            CStatusControl::setup

****************************************************************************/

BOOL CStatusControl::setup(CStatusBar * parent, UINT id, CRect & r)
{
     int i = parent->CommandToIndex(id);

     parent->GetItemRect(i, &r);

     parent->SetPaneText(i, _T(""));

     
     if(r.IsRectEmpty())
	{ /* offscreen */
	 CRect r1;
	 parent->GetWindowRect(&r1); // get parent width
	 r.left = r1.right + 1;
	 r.top =  r1.top;
	 r.right = r1.right + 2;
	 r.bottom = r1.bottom;
	 return FALSE;
	} /* offscreen */

     return TRUE;
 }

/****************************************************************************
*                         CStatusControl::reposition

****************************************************************************/

void CStatusControl::reposition(CWnd * wnd)
{
     if(wnd == NULL || wnd->m_hWnd == NULL)
         return;
     UINT id = ::GetWindowLong(wnd->m_hWnd, GWL_ID);
     CRect r;

     // Note that because the control ID is the same as the
     // pane ID, this actually works well enough that
     // no special variable is required to obtain the
     // pane index.
     CStatusBar * parent = (CStatusBar *)wnd->GetParent();
     int i = parent->CommandToIndex(id);
     parent->GetItemRect(i, &r);
     wnd->SetWindowPos(&wndTop, r.left, r.top, r.Width(), r.Height(), 0);
}
/****************************************************************************
*                              CStatusControl::Create

****************************************************************************/

BOOL CStatusControl::Create(LPCTSTR classname, CStatusBar * parent, UINT id, DWORD style)
{
     CRect r;

     setup(parent, id, r);

     return CWnd::Create(classname, NULL, style | WS_CHILD, r, parent, id);
}

void CStatusControl::Reposition()
{
    reposition(this);
}


//////////////////////////////////////////////////////////////////////////
//CStatusButton

CStatusButton::CStatusButton()
{
	m_bCancel=FALSE;
}

CStatusButton::~CStatusButton()
{
}

/*BOOL CStatusButton::Create(CStatusBar *parent, UINT id, DWORD style)
{

}*/
BEGIN_MESSAGE_MAP(CStatusButton, CButton)
//{{AFX_MSG_MAP(CStatusButton)
ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CStatusButton::Create(CStatusBar * parent, CRect rc, UINT id, DWORD style)
{
	CRect r;
	
    CStatusControl::setup(parent, 0, r);
	int x=rc.right;	
	r.top=r.top-1;
	r.bottom=r.bottom+1;
	r.left=x+20;
	r.right=r.left+60;
	BOOL result = CButton::Create(_T("取消"),style| WS_CHILD,r,parent,id);//CButton::Create(style | WS_CHILD, r, parent, id);
    if(!result)
	{
		return FALSE;
	}
	
    CFont * f = parent->GetFont();
    SetFont(f);
    return TRUE;
}



void CStatusButton::OnClicked() 
{
	// TODO: Add your control notification handler code here		
	int i=AfxMessageBox(_T("取消?"),MB_YESNO);
	if(i==IDYES)
	{
		m_bCancel=TRUE;
	}
}


IMPLEMENT_DYNCREATE(CProgressBar, CProgressCtrl)

BEGIN_MESSAGE_MAP(CProgressBar, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressBar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CProgressBar::CProgressBar()
{
	m_Rect.SetRect(0,0,0,0);
	m_iCancel=0;
}

CProgressBar::CProgressBar(LPCTSTR strMessage, int nSize /*=100*/, 
						   int MaxValue /*=100*/, BOOL bSmooth /*=FALSE*/, 
                           int nPane /*=0*/)
{
	Create(strMessage, nSize, MaxValue, bSmooth, nPane);
}

CProgressBar::~CProgressBar()
{
	Clear();
}

CStatusBar* CProgressBar::GetStatusBar()
{
	CWnd *pMainWnd = AfxGetMainWnd();
	if (!pMainWnd)
		return NULL;

    // If main window is a frame window, use normal methods...
    if (pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
    {
        CWnd* pMessageBar = ((CFrameWnd*)pMainWnd)->GetMessageBar();
        return DYNAMIC_DOWNCAST(CStatusBar, pMessageBar);
    }
    // otherwise traverse children to try and find the status bar...
    else
	    return DYNAMIC_DOWNCAST(CStatusBar, 
                                pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR));
}

// Create the CProgressCtrl as a child of the status bar positioned
// over the first pane, extending "nSize" percentage across pane.
// Sets the range to be 0 to MaxValue, with a step of 1.
BOOL CProgressBar::Create(LPCTSTR strMessage, int nSize /*=100*/, 
						  int MaxValue /*=100*/, BOOL bSmooth /*=FALSE*/, 
                          int nPane /*=0*/)
{
	BOOL bSuccess = FALSE;
	
	CStatusBar *pStatusBar = GetStatusBar();
	if (!pStatusBar)
		return FALSE;

	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
#ifdef PBS_SMOOTH	 
	if (bSmooth)
		dwStyle |= PBS_SMOOTH;
#endif

	// Get CRect coordinates for requested status bar pane
	CRect PaneRect;
	pStatusBar->GetItemRect(nPane, &PaneRect);
	
	// Create the progress bar
	bSuccess = CProgressCtrl::Create(dwStyle, PaneRect, pStatusBar, 1);
	ASSERT(bSuccess);
	if (!bSuccess)
		return FALSE;

	// Set range and step
	SetRange(0, MaxValue);
	SetStep(1);

	m_strMessage  = strMessage;
	m_nSize 	  = nSize;
	m_nPane 	  = nPane;
    m_strPrevText = pStatusBar->GetPaneText(m_nPane); 

	// Resize the control to its desired width
	Resize();
	m_cStatusButton.Create(pStatusBar,m_Rect, 0, WS_VISIBLE);
	return TRUE;
}

void CProgressBar::Clear()
{
	if (!IsWindow(GetSafeHwnd()))
		return;

	// Hide the window. This is necessary so that a cleared
	// window is not redrawn if "Resize" is called
	ModifyStyle(WS_VISIBLE, 0);

	CString strRes;
	if (m_nPane == 0)
		strRes.LoadString(AFX_IDS_IDLEMESSAGE);   // Get the IDLE_MESSAGE
      else
		strRes = m_strPrevText;                   // Restore previous text

	// Place the IDLE_MESSAGE in the status bar 
	CStatusBar *pStatusBar = GetStatusBar();
	if (pStatusBar)
	{
		pStatusBar->SetPaneText(m_nPane, strRes);
		pStatusBar->UpdateWindow();
	}
}

BOOL CProgressBar::SetText(LPCTSTR strMessage)
{ 
	m_strMessage = strMessage; 
	return Resize(); 
}

BOOL CProgressBar::SetSize(int nSize)
{
	m_nSize = nSize; 
	return Resize();
}

COLORREF CProgressBar::SetBarColour(COLORREF clrBar)
{
#ifdef PBM_SETBKCOLOR
	if (!IsWindow(GetSafeHwnd()))
          return CLR_DEFAULT;

	return SendMessage(PBM_SETBARCOLOR, 0, (LPARAM) clrBar);
#else
	UNUSED(clrBar);
	return CLR_DEFAULT;
#endif
}

COLORREF CProgressBar::SetBkColour(COLORREF clrBk)
{
#ifdef PBM_SETBKCOLOR
	if (!IsWindow(GetSafeHwnd()))
		return CLR_DEFAULT;

	return SendMessage(PBM_SETBKCOLOR, 0, (LPARAM) clrBk);
#else
	UNUSED(clrBk);
	return CLR_DEFAULT;
#endif
}

BOOL CProgressBar::SetRange(int nLower, int nUpper, int nStep /* = 1 */)	
{	 
	if (!IsWindow(GetSafeHwnd()))
		return FALSE;

	// To take advantage of the Extended Range Values we use the PBM_SETRANGE32
	// message intead of calling CProgressCtrl::SetRange directly. If this is
	// being compiled under something less than VC 5.0, the necessary defines
	// may not be available.

#ifdef PBM_SETRANGE32
	ASSERT(-0x7FFFFFFF <= nLower && nLower <= 0x7FFFFFFF);
	ASSERT(-0x7FFFFFFF <= nUpper && nUpper <= 0x7FFFFFFF);
	SendMessage(PBM_SETRANGE32, (WPARAM) nLower, (LPARAM) nUpper);
#else
	ASSERT(0 <= nLower && nLower <= 65535);
	ASSERT(0 <= nUpper && nUpper <= 65535);
	CProgressCtrl::SetRange(nLower, nUpper);
#endif
	
	CProgressCtrl::SetStep(nStep);
	return TRUE;
}

int CProgressBar::SetPos(int nPos)	   
{
	if (!IsWindow(GetSafeHwnd()))
		return 0;

#ifdef PBM_SETRANGE32
	ASSERT(-0x7FFFFFFF <= nPos && nPos <= 0x7FFFFFFF);
#else
	ASSERT(0 <= nPos && nPos <= 65535);
#endif

	ModifyStyle(0,WS_VISIBLE);
	return CProgressCtrl::SetPos(nPos);
}

int CProgressBar::OffsetPos(int nPos) 
{ 
	if (!IsWindow(GetSafeHwnd()))
		return 0;

	ModifyStyle(0,WS_VISIBLE);
	return CProgressCtrl::OffsetPos(nPos);
}

int CProgressBar::SetStep(int nStep)
{ 
	if (!IsWindow(GetSafeHwnd()))
		return 0;

	ModifyStyle(0,WS_VISIBLE);
	return CProgressCtrl::SetStep(nStep);	 
}

int CProgressBar::StepIt(BOOL& bCancel)
{ 
	if (!IsWindow(GetSafeHwnd()))
		return 0;

	ModifyStyle(0,WS_VISIBLE);
	int iRe=CProgressCtrl::StepIt();
	bCancel=m_cStatusButton.m_bCancel;
	return iRe;    
}

BOOL CProgressBar::Resize() 
{
	if (!IsWindow(GetSafeHwnd()))
		return FALSE;

	CStatusBar *pStatusBar = GetStatusBar();
	if (!pStatusBar)
		return FALSE;

	// Redraw the window text
	if (IsWindowVisible())
	{
		pStatusBar->SetPaneText(m_nPane, m_strMessage);
		pStatusBar->UpdateWindow();
	}

	// Calculate how much space the text takes up
	CClientDC dc(pStatusBar);
	CFont *pOldFont = dc.SelectObject(pStatusBar->GetFont());
	CSize size = dc.GetTextExtent(m_strMessage);		// Length of text
	int margin = dc.GetTextExtent(_T(" ")).cx * 2;		// Text margin
	dc.SelectObject(pOldFont);

	// Now calculate the rectangle in which we will draw the progress bar
	CRect rc;
	pStatusBar->GetItemRect(m_nPane, rc);

	// Position left of progress bar after text and right of progress bar
	// to requested percentage of status bar pane
	if (!m_strMessage.IsEmpty())
	    rc.left += (size.cx + 2*margin);
	rc.right -= (rc.right - rc.left) * (100 - m_nSize) / 100;

	if (rc.right < rc.left) rc.right = rc.left;
	
	// Leave a litle vertical margin (10%) between the top and bottom of the bar
	int Height = rc.bottom - rc.top;
	rc.bottom -= Height/10;
	rc.top	  += Height/10;

	// If the window size has changed, resize the window
	if (rc != m_Rect)
	{
		MoveWindow(&rc);		
		if(m_cStatusButton.m_hWnd!=NULL)
		{
			int x=rc.right;	
			CRect rButton(rc);
			rButton.left=rc.right+20;
			rButton.right=rButton.left+60;
			CRect r;
			pStatusBar->GetItemRect(0,&r);
			if(rButton.right>r.right)
			{				
				rButton.right=r.right;
			}		
			m_cStatusButton.MoveWindow(&rButton);
		}
		m_Rect = rc;
	}

	return TRUE;
}

BOOL CProgressBar::OnEraseBkgnd(CDC* pDC) 
{
	Resize();
	return CProgressCtrl::OnEraseBkgnd(pDC);
}