#include "stdafx.h"
#include "JahCoverToolTipXye.h"

#define MYTIP_NAME "multi_tip"
#define TIMER_INITIAL 1
#define TIMER_AUTOPOP 4
#define TIMER_RESHOW 3
#define ID_BTN_TIP 152

//##ModelId=4140A36800A6
CJahCoverToolTip::CJahCoverToolTip():m_rcMargin(0,0,0,0)
{
	RegisterWindowClass();
	m_nTimeAutoPop=2000;
	m_nTimeReShow=100;
	m_nTimeInitial=800;
	m_bTimerInitial=FALSE;
	m_bTimerPop=FALSE;
	m_hCurWnd=NULL;
	m_crBg=::GetSysColor(COLOR_INFOBK);
	m_crText=::GetSysColor(COLOR_INFOTEXT);
}
//##ModelId=4140A3680076
CJahCoverToolTip::~CJahCoverToolTip()
{	
}
BEGIN_MESSAGE_MAP(CJahCoverToolTip, CWnd)
	//{{AFX_MSG_MAP(CJahCoverToolTip)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_BTN_TIP,OnCmdTip)
END_MESSAGE_MAP()

//##ModelId=4140A3680072
void CJahCoverToolTip::RegisterWindowClass()
{	WNDCLASS wc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground=HBRUSH(COLOR_WINDOW+1);
	wc.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=NULL;
	wc.hInstance=::AfxGetInstanceHandle();
	wc.lpfnWndProc=::DefWindowProc;
	wc.lpszClassName=MYTIP_NAME;
	wc.lpszMenuName=NULL;
	wc.style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;	
	::RegisterClass(&wc);
}
//##ModelId=4140A368007C
BOOL CJahCoverToolTip::Create(CWnd *pParent)
{
	BOOL bRet = CWnd::CreateEx(NULL, MYTIP_NAME, NULL, WS_POPUP, 0, 0, 0, 0, 
							   pParent->GetSafeHwnd(), NULL, NULL );
	if(bRet)
	{
		SetOwner(pParent);
		CRect rc(0,0,0,0);
		m_btn.Create(NULL,BS_PUSHBUTTON|BS_FLAT,rc,this,ID_BTN_TIP);
	}	
	return bRet;
}
//##ModelId=4140A3680081
void CJahCoverToolTip::SetDelayTime(UINT nDelay)
{
	m_nTimeAutoPop=nDelay;
	m_nTimeReShow=nDelay;
	m_nTimeInitial=nDelay;	
}
//##ModelId=4140A368007E
void CJahCoverToolTip::SetDelayTime(DWORD dwDuration, int iTime)
{
	switch(dwDuration)
	{
	case TTDT_AUTOPOP:
		m_nTimeAutoPop=iTime;
		break;
	case TTDT_RESHOW:
		m_nTimeReShow=iTime;
		break;
	case TTDT_INITIAL:
		m_nTimeInitial=iTime;
		break;
	}
}
//##ModelId=4140A3680086
BOOL CJahCoverToolTip::AddTool(CWnd *pWnd, LPCTSTR lpszText)
{
	char *pText;
	if(m_map.Lookup(pWnd->GetSafeHwnd(),(void*&)pText))
	{
		m_map.RemoveKey(pWnd->GetSafeHwnd());
		if(pText!=LPSTR_TEXTCALLBACK)
			delete []pText;
	}
	
	if(lpszText==LPSTR_TEXTCALLBACK)
	{
		m_map.SetAt(pWnd->GetSafeHwnd(),(void*)lpszText);
	}
	else
	{
		pText=new char[strlen(lpszText)+1];
		ASSERT(pText);
		strcpy(pText,lpszText);
		m_map.SetAt(pWnd->GetSafeHwnd(),pText);
	}
	return TRUE;
}
//##ModelId=4140A3680089
int CJahCoverToolTip::GetDelayTime(DWORD dwDuration) const
{
	switch(dwDuration)
	{
	case TTDT_AUTOPOP:
		return	m_nTimeAutoPop;
		break;
	case TTDT_RESHOW:
		return m_nTimeReShow;
		break;
	case TTDT_INITIAL:
		return m_nTimeInitial;
	default:
		return 0;
		break;
	}
}
//##ModelId=4140A3680073
void CJahCoverToolTip::Hide()
{
	if(m_bTimerInitial)//OnTimer(1)还未执行
	{
		KillTimer(TIMER_INITIAL);
		m_bTimerInitial=FALSE;
	}
	if(IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
		KillTimer(TIMER_AUTOPOP);
		m_bTimerPop=FALSE;//m_bTimerPop为True必在OnTimer(TIMER_INITIAL)之后窗口必然可见
	}	
}
//##ModelId=4140A368008C
void CJahCoverToolTip::RelayEvent(LPMSG lpMsg)
{
	char *pText;
	static CPoint ptLast(lpMsg->pt);
	if(lpMsg->hwnd==m_hWnd||lpMsg->hwnd==m_btn.m_hWnd)
		return;
	switch(lpMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		Hide();
		break;
	case WM_MOUSEMOVE:
		if(lpMsg->pt.x=ptLast.x&&lpMsg->pt.y==ptLast.y)
			 	return;                               
	    if(m_hCurWnd==NULL)
		{
			if(m_map.Lookup(lpMsg->hwnd,(void*&)pText))//鼠标在工具窗口内
			{
				m_hCurWnd=lpMsg->hwnd;
				SetTimer(TIMER_INITIAL,m_nTimeInitial,NULL);
				m_bTimerInitial=TRUE;
			}
		}
		else
		{
			if(m_hCurWnd==lpMsg->hwnd)
			{
				//在当前工具窗口内移动
				if(m_bTimerInitial)//还没有显示
				{
					KillTimer(TIMER_INITIAL);
					SetTimer(TIMER_INITIAL,m_nTimeInitial,NULL);
				}
			}
			else
			{
				Hide();
				m_hCurWnd=NULL;
			}
		}				
		break;
	default:
		break;

	}
	ptLast=lpMsg->pt;
}
//##ModelId=4140A368006D
void CJahCoverToolTip::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_INITIAL:
		Show();
		KillTimer(TIMER_INITIAL);
		m_bTimerInitial=FALSE;
		SetTimer(TIMER_AUTOPOP,m_nTimeAutoPop,NULL);
		m_bTimerPop=TRUE;
		break;
	case TIMER_AUTOPOP:
		Hide();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
//##ModelId=4140A3680074
void CJahCoverToolTip::Show()
{
	if(!m_hCurWnd)
		return;
	char *pText;
	m_map.Lookup(m_hCurWnd,(void*&)pText);
	char tstr[1024];
	if(pText==LPSTR_TEXTCALLBACK)
	{
		TOOLTIPTEXT ttt;
		ttt.hdr.hwndFrom=m_hWnd;
		ttt.hdr.idFrom=(UINT)m_hCurWnd;
		ttt.hdr.code=TTN_NEEDTEXT;
		ttt.uFlags=TTF_IDISHWND;
		ttt.lpszText=m_strText;
		GetOwner()->SendMessage(WM_NOTIFY,NULL,(LPARAM)&ttt);
	}
	else
		strcpy(m_strText,pText);
	
	HDC hDC=::GetDC(m_hWnd);
	HGDIOBJ hGdi=GetStockObject(ANSI_FIXED_FONT);
	::SelectObject(hDC, hGdi);
	
	int len=strlen(m_strText);
	char *p=m_strText,*pOld,*pEnd=m_strText+len;
	CSize sz(0,0),szTemp;
	do
	{
		pOld=p;
		p=strchr(p,'\n');
		if(p)
		{
			strncpy(tstr,pOld,p-pOld);
			tstr[p-pOld]=0;
			::GetTextExtentPoint(hDC,tstr,p-pOld,&szTemp);
			sz.cy+=szTemp.cy;
			if(sz.cx<szTemp.cx)
				sz.cx=szTemp.cx;
			p++;
		}
	}while(p);	
	if(pEnd-pOld>=2)
	{
		strncpy(tstr,pOld,pEnd-pOld);
		::GetTextExtentPoint(hDC,tstr,pEnd-pOld-1,&szTemp);
		sz.cy+=szTemp.cy;
		if(sz.cx<szTemp.cx)
			sz.cx=szTemp.cx;
	}
	::ReleaseDC(m_hWnd,hDC);	
	sz.cx+=5;
	sz.cy+=5;		
	sz.cx+=m_rcMargin.top+m_rcMargin.bottom;
	sz.cy+=m_rcMargin.bottom+m_rcMargin.top;
	CRect rc(1,1,m_rcMargin.left-1,m_rcMargin.top-1);
	rc.NormalizeRect();
	m_btn.MoveWindow(&rc);
	m_btn.ShowWindow(SW_SHOW);
	CPoint pt;
	::GetCursorPos(&pt);
	pt.x+=::GetSystemMetrics(SM_CXCURSOR)/2;
	pt.y+=::GetSystemMetrics(SM_CYCURSOR)/2;
	MoveWindow(pt.x,pt.y,sz.cx,sz.cy);
	ShowWindow(SW_SHOWNOACTIVATE);
	UpdateWindow();	
}
//##ModelId=4140A368006B
void CJahCoverToolTip::OnDestroy() 
{

	if(m_bTimerInitial)
		KillTimer(TIMER_INITIAL);
	if(m_bTimerPop)
		KillTimer(TIMER_AUTOPOP);

	char *pText;
	HWND hwnd;
	for (POSITION pos = m_map.GetStartPosition(); pos != NULL;)
	{
		m_map.GetNextAssoc(pos,(void*&)hwnd,(void*&)pText);
		if(pText!=LPSTR_TEXTCALLBACK)
		{
			delete []pText;
		}
	}
	m_map.RemoveAll();
	CWnd::OnDestroy();	
}
//##ModelId=4140A3680069
void CJahCoverToolTip::OnPaint() 
{
	CPaintDC dc(this); 
	CRect rc;

	GetClientRect(&rc);
	HGDIOBJ	hGdi=GetStockObject(ANSI_FIXED_FONT);
	dc.SelectObject(hGdi);	
	dc.FillSolidRect(&rc,m_crBg);
	rc.bottom--;
	rc.right--;
	dc.MoveTo(rc.left,rc.top);
	dc.LineTo(rc.right,rc.top);
	dc.LineTo(rc.right,rc.bottom);
	dc.LineTo(rc.left,rc.bottom);
	dc.LineTo(rc.left,rc.top);
	rc.top++;
	rc.left++;
	rc.right--;
	rc.bottom--;
	rc.top+=m_rcMargin.top;
	rc.bottom-=m_rcMargin.bottom;
	rc.left+=m_rcMargin.left;
	rc.right-=m_rcMargin.right;
	dc.SetTextColor(m_crText);	
	dc.DrawText(m_strText,strlen(m_strText),&rc,0);
}
//##ModelId=4140A3680090
void CJahCoverToolTip::DelTool(CWnd *pWnd)
{
	char *pText;
	if(m_map.Lookup(pWnd->GetSafeHwnd(),(void*&)pText))
	{
		m_map.RemoveKey(pWnd->GetSafeHwnd());
		if(pText!=LPSTR_TEXTCALLBACK)
			delete []pText;
		if(pWnd->GetSafeHwnd()==m_hCurWnd)
			Hide();
	}
}
//##ModelId=4140A3680092
void CJahCoverToolTip::SetTipTextColor(COLORREF clr)
{
	m_crText=clr;
}
//##ModelId=4140A3680094
void CJahCoverToolTip::SetTipBkColor(COLORREF clr)
{
	m_crBg=clr;
}
//##ModelId=4140A368009A
COLORREF CJahCoverToolTip::GetTipBkColor() const
{
	return m_crBg;
}
//##ModelId=4140A368009C
COLORREF CJahCoverToolTip::GetTipTextColor() const
{
	return m_crText;
}
//##ModelId=4140A368009E
void CJahCoverToolTip::SetMargin(LPRECT lprc)
{
	m_rcMargin.CopyRect(lprc);
}
//##ModelId=4140A36800A0
void CJahCoverToolTip::GetMargin(LPRECT lprc) const
{
	::CopyRect(lprc,&m_rcMargin);
}
//##ModelId=4140A3680075
void CJahCoverToolTip::OnCmdTip()
{
	ShellExecute(NULL, _T("open"), "http://www.softjah.com", NULL,NULL, SW_SHOWNORMAL);
}
//##ModelId=4140A36800A5
void CJahCoverToolTip::EnableToolTip()
{
	SetDelayTime(TTDT_INITIAL,100);  
	SetDelayTime(TTDT_AUTOPOP,5000);
	SetTipBkColor(RGB(243,249,228));
	SetTipTextColor(RGB(0,0,0));
}


