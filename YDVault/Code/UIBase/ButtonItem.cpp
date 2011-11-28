#include "StdAfx.h"
#include "ButtonItem.h"
////////////////////////////////////////////////////////////////////////////////
// CButtonItem Class

CButtonItem::CButtonItem (LPCTSTR lpszText, UINT id,CBCGPGridCtrl* _pGridCtrl,CBCGPGridRow* _pRow,CWnd* _pWnd /*= NULL*/) :
CBCGPGridItem (lpszText)
{
	m_id = id;
	m_pSendWnd = _pWnd;
	m_pRow = _pRow;
	m_pGridCtrl = _pGridCtrl;
}
//*****************************************************************************************
void CButtonItem::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID (pDC);

	OnFillBackground (pDC, rect);

	rect.DeflateRect (1, 1);

	pDC->FillRect (rect, &globalData.brBarFace);

	pDC->Draw3dRect (rect, globalData.clrBarLight, globalData.clrBarDkShadow);
	rect.DeflateRect (1, 1);
	pDC->Draw3dRect (rect, globalData.clrBarHilite, globalData.clrBarShadow);

	COLORREF clrText = pDC->SetTextColor (globalData.clrBarText);

	CString strText = (LPCTSTR)(_bstr_t) m_varValue;

	pDC->DrawText (strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	pDC->SetTextColor (clrText);
}
//*****************************************************************************************
BOOL CButtonItem::OnClickValue (UINT uiMsg, CPoint point)
{
	if (uiMsg != WM_LBUTTONUP)
	{
		return CBCGPGridItem::OnClickValue (uiMsg, point);
	}

	if(m_pSendWnd == NULL)
	{
		CBCGPGridCtrl* pGridCtrl = GetOwnerList();
		ASSERT_VALID (pGridCtrl);
		m_pSendWnd = pGridCtrl;
		
	}
	ASSERT(m_pSendWnd);
	ASSERT(m_pGridCtrl);
	ASSERT(m_pRow);
	m_pGridCtrl->SetCurSel(m_pRow);
	

	m_pSendWnd->SendMessage (WM_COMMAND,
		MAKEWPARAM (m_id, BN_CLICKED));

	return TRUE;
}

