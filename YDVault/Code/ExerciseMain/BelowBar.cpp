// BelowBar.cpp : implementation file
//

#include "stdafx.h"
#include "BelowBar.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBelowBar

CBelowBar::CBelowBar()
{
}

CBelowBar::~CBelowBar()
{
}


BEGIN_MESSAGE_MAP(CBelowBar, CDialogBar)
	//{{AFX_MSG_MAP(CBelowBar)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBelowBar message handlers

void CBelowBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
 	// TODO: Add your message handler code here and/or call default
  	CRect rect=lpncsp->rgrc[0];
  	rect.DeflateRect(3,0,0,0);
  	lpncsp->rgrc[0]=rect;
	
	CDialogBar::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CBelowBar::OnNcPaint() 
{
	// TODO: Add your message handler code here
	CWindowDC dc(this);
 	CRect rect;
	GetWindowRect(&rect);
	CPen pen(PS_SOLID,1,RGB(255,255,255));
	rect.OffsetRect(-rect.TopLeft());
	CPen* oldpen = dc.SelectObject(&pen);
	dc.MoveTo(2,0);
	dc.LineTo(2,rect.Height()-2);
	pen.DeleteObject();
	dc.SelectObject(oldpen);
	ReleaseDC(&dc);

	// Do not call CDialogBar::OnNcPaint() for painting messages
}

void CBelowBar::ShowBegin(OPERATION type)
{
	if(type == OP_NEW || type == OP_EDIT)
	{
		GetDlgItem(ID_BLEW_PROPERTY_CANCEL)->SetWindowText(_T("取消"));
	}
	else
	{
		GetDlgItem(ID_BLEW_PROPERTY_CANCEL)->SetWindowText(_T("关闭"));
	}
}

void CBelowBar::SaveCompleted()
{
	GetDlgItem(ID_BLEW_PROPERTY_CANCEL)->SetWindowText(_T("关闭"));
}

void CBelowBar::UpdateCompleted()
{
	GetDlgItem(ID_BLEW_PROPERTY_CANCEL)->SetWindowText(_T("关闭"));
}