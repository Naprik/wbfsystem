#include "stdafx.h"
#include "JahCoverSystemTray.h"

//##ModelId=4140A4D20206
CJahCoverSystemTray::CJahCoverSystemTray()
{
    Initialise();
}

//##ModelId=4140A4D20207
CJahCoverSystemTray::CJahCoverSystemTray(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, 
                         HICON icon, UINT uID)
{
    Initialise();
    Create(pParent, uCallbackMessage, szToolTip, icon, uID);
}

//##ModelId=4140A4D201EA
void CJahCoverSystemTray::Initialise()
{
    memset(&m_tnd, 0, sizeof(m_tnd));
    m_bEnabled				= FALSE;
    m_bHidden				= FALSE;
    m_DefaultMenuItemID		= 0;
    m_DefaultMenuItemByPos	= TRUE;
}

//##ModelId=4140A4D201FC
BOOL CJahCoverSystemTray::Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szToolTip, 
                         HICON icon, UINT uID)
{
    m_bEnabled = (GetVersion() & 0xff) >= 4;
    if (!m_bEnabled) 
    {
        ASSERT(FALSE);
        return FALSE;
    }

    ASSERT(uCallbackMessage >= WM_USER);

    ASSERT(_tcslen(szToolTip) <= 64);

    m_tnd.cbSize = sizeof(NOTIFYICONDATA);
    m_tnd.hWnd   = pParent->GetSafeHwnd();
    m_tnd.uID    = uID;
    m_tnd.hIcon  = icon;
    m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_tnd.uCallbackMessage = uCallbackMessage;
    _tcscpy(m_tnd.szTip, szToolTip);

    m_bEnabled = Shell_NotifyIcon(NIM_ADD, &m_tnd);
    ASSERT(m_bEnabled);
    return m_bEnabled;
}

//##ModelId=4140A4D20204
CJahCoverSystemTray::~CJahCoverSystemTray()
{
    RemoveIcon();
}

//##ModelId=4140A4D201F6
void CJahCoverSystemTray::RemoveIcon()
{
    if (!m_bEnabled) return;

    m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
    m_bEnabled = FALSE;
}

//##ModelId=4140A4D201F9
void CJahCoverSystemTray::HideIcon()
{
    if (m_bEnabled && !m_bHidden) {
        m_tnd.uFlags = NIF_ICON;
        Shell_NotifyIcon (NIM_DELETE, &m_tnd);
        m_bHidden = TRUE;
    }
}

//##ModelId=4140A4D201FA
BOOL CJahCoverSystemTray::SetIcon(UINT nIDResource)
{
    HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

    if (!m_bEnabled) return FALSE;

    m_tnd.uFlags = NIF_ICON;
    m_tnd.hIcon = hIcon;

    return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

//##ModelId=4140A4D201F3
BOOL CJahCoverSystemTray::SetMenuDefaultItem(UINT uItem, BOOL bByPos)
{
    if ((m_DefaultMenuItemID == uItem) && (m_DefaultMenuItemByPos == bByPos)) 
        return TRUE;

    m_DefaultMenuItemID = uItem;
    m_DefaultMenuItemByPos = bByPos;   

    CMenu menu, *pSubMenu;

    if (!menu.LoadMenu(m_tnd.uID))
        return FALSE;

    pSubMenu = menu.GetSubMenu(0);
    if (!pSubMenu)
        return FALSE;

    ::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

    return TRUE;
}

LRESULT CJahCoverSystemTray::OnTrayNotification(UINT wParam, LONG lParam) 
{
    if (wParam != m_tnd.uID)
        return 0;

    CMenu menu, *pSubMenu;
    CWnd* pTarget			= AfxGetMainWnd();
	static BOOL IsShowed	= FALSE;

    if (LOWORD(lParam) == WM_RBUTTONUP)
    {    
        if (!menu.LoadMenu(m_tnd.uID))
            return 0;
        
        pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return 0;

        ::SetMenuDefaultItem(pSubMenu->m_hMenu, m_DefaultMenuItemID, m_DefaultMenuItemByPos);

        CPoint pos;
        GetCursorPos(&pos);

        pTarget->SetForegroundWindow();  
		IsShowed = TRUE;
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, 
                         pTarget->GetSafeHwnd(), NULL);
 		IsShowed = FALSE;
        pTarget->PostMessage(WM_NULL, 0, 0);

        menu.DestroyMenu();
    } 
    else if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
    {
		if(IsShowed)
			return 0;
        pTarget->SetForegroundWindow();  

        UINT uItem;
        if (m_DefaultMenuItemByPos)
        {
           if (!menu.LoadMenu(m_tnd.uID))
                return 0;
            
            pSubMenu = menu.GetSubMenu(0);
            if (!pSubMenu)
                return 0;
            
            uItem = pSubMenu->GetMenuItemID(m_DefaultMenuItemID);
        }
        else
            uItem = m_DefaultMenuItemID;
        
        pTarget->SendMessage(WM_COMMAND, uItem, 0);

        menu.DestroyMenu();
    }

    return 1;
}
