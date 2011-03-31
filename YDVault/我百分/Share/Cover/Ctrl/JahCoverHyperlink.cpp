#include "stdafx.h"
#include "JahCoverHyperLink.h"

//##ModelId=4140A367020B
CJahCoverHyperLink::CJahCoverHyperLink()
{
    m_hLinkCursor		= NULL;                
    m_LinkColor			= RGB(0, 0, 255);  
    m_VisitedColor		= RGB(0, 0, 255);  
    m_HoverColor		= RGB(242, 101, 34);
    m_sUrl		        = "";
	m_bIsHover			= FALSE;
	m_bIsVisited		= FALSE;
	m_bUnderline		= TRUE;
}

//##ModelId=4140A3670206
CJahCoverHyperLink::~CJahCoverHyperLink()
{
    m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CJahCoverHyperLink, CStatic)
    //{{AFX_MSG_MAP(CJahCoverHyperLink)
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//##ModelId=4140A36701E6
void CJahCoverHyperLink::OnClicked()
{
    HINSTANCE result = ShellExecute(NULL
	   , _T("open"), m_sUrl, NULL,NULL, SW_SHOW);

	m_bIsVisited = TRUE;
}

//##ModelId=4140A36701F8
HBRUSH CJahCoverHyperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    if (m_bIsHover)
        pDC->SetTextColor(m_HoverColor);
    else if (m_bIsVisited)
        pDC->SetTextColor(m_VisitedColor);
    else
        pDC->SetTextColor(m_LinkColor);

    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

//##ModelId=4140A36701ED
void CJahCoverHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
    CStatic::OnMouseMove(nFlags, point);

    if (m_bIsHover)        
    {
        CRect rect;
        GetClientRect(rect);

        if (!rect.PtInRect(point))
        {
            m_bIsHover = FALSE;
            ReleaseCapture();
            RedrawWindow();
            return;
        }
    }
    else                      
    {
        m_bIsHover = TRUE;
        RedrawWindow();
        SetCapture();
    }
}

//##ModelId=4140A36701F1
BOOL CJahCoverHyperLink::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message) 
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

//##ModelId=4140A36701FE
BOOL CJahCoverHyperLink::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

//##ModelId=4140A36701FC
void CJahCoverHyperLink::PreSubclassWindow() 
{
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
    
    if (m_sUrl.IsEmpty())
        GetWindowText(m_sUrl);

    CString strWndText;
    GetWindowText(strWndText);
    if (strWndText.IsEmpty()) 
        SetWindowText(m_sUrl);

    LOGFONT lf;
    GetFont()->GetLogFont(&lf);
    lf.lfUnderline = m_bUnderline;
    m_Font.CreateFontIndirect(&lf);
    SetFont(&m_Font);

    SetCursor();     

    CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_sUrl, rect, 1);

    CStatic::PreSubclassWindow();
}

//##ModelId=4140A3670205
void CJahCoverHyperLink::SetCursor()
{
    if (m_hLinkCursor == NULL)                
    {
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hLinkCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
    }
}
//=====================================================================
// public function
//##ModelId=4140A3670203
void CJahCoverHyperLink::SetURL(CString sUrl)
{
    m_sUrl = sUrl;

    if (::IsWindow(GetSafeHwnd())) 
        m_ToolTip.UpdateTipText(sUrl, this, 1);
}
