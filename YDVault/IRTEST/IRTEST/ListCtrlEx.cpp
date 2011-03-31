#include "stdafx.h"
#include <assert.h>
#include "ListCtrlEx.h"


IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()
CListCtrlEx::CListCtrlEx()
{
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_clrHText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrHBkgnd = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrPercent = RGB(255,0,0);//::GetSysColor(COLOR_HIGHLIGHT);
	m_clrHPercent = ::GetSysColor(COLOR_BTNFACE);
}
CListCtrlEx::~CListCtrlEx()
{
}
void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
    switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	   *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
 	    *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		{
			int iCol = lplvcd->iSubItem;
			if ( iCol == 0 )
				break;
			int iRow = lplvcd->nmcd.dwItemSpec;
			CString sItem = GetItemText(iRow, iCol);
			CRect rcItem;
			GetCellRect(iRow, iCol, LVIR_BOUNDS, rcItem);
			
			CDC *pDC= CDC::FromHandle(lplvcd->nmcd.hdc);
			int State = ListView_GetItemState(m_hWnd, iRow, LVIS_CUT|LVIS_SELECTED|LVIS_FOCUSED);

			pDC->FillSolidRect(&rcItem, (State &LVIS_SELECTED) ? m_clrHBkgnd : m_clrBkgnd);
			rcItem.DeflateRect(2, 1);
			if(sItem.Right(1) == '%')
			{
				pDC->Rectangle(rcItem);
				rcItem.DeflateRect(1, 1);
				CRect rcLeft, rcRight;
				rcLeft = rcRight = rcItem;
				rcLeft.right = rcLeft.left + MulDiv(_ttoi(sItem), rcItem.Width(), 100);
				rcRight.left = rcLeft.right;
				if((State &LVIS_SELECTED))
				{
					pDC->FillSolidRect(rcLeft, m_clrHPercent);
					pDC->FillSolidRect(rcRight, m_clrBkgnd);
					pDC->SetTextColor(m_clrText);
					pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
				}
				else
				{
					pDC->FillSolidRect(rcLeft, m_clrPercent);
					pDC->FillSolidRect(rcRight, m_clrHPercent);
					CRgn rgn;
					rgn.CreateRectRgnIndirect(rcLeft);
					pDC->SelectClipRgn(&rgn);
					pDC->SetTextColor(m_clrBkgnd);
					pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

					rgn.SetRectRgn(rcRight);
					pDC->SelectClipRgn(&rgn);
					pDC->SetTextColor(m_clrText);
					pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
					pDC->SelectClipRgn(NULL);
				}
			}
			else
			{
				pDC->SetTextColor((State & LVIS_SELECTED) ? m_clrHText:m_clrText);
				pDC->DrawText(sItem, -1, &rcItem, DT_NOCLIP | DT_LEFT | DT_VCENTER);
			}
		}
		*pResult= CDRF_SKIPDEFAULT;
		break;
	}
}

BOOL CListCtrlEx::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
{
	if(iCol)
		return GetSubItemRect(iRow, iCol, nArea, rect);
	
	if(GetHeaderCtrl()->GetItemCount() == 1)
		return GetItemRect(iRow, rect, nArea);
	
	CRect rCol1;
	if(!GetSubItemRect(iRow, 1, nArea, rCol1))
		return FALSE;
	
	if(!GetItemRect(iRow, rect, nArea))
		return FALSE;
	
	rect.right = rCol1.left;
	
	return TRUE;
}
