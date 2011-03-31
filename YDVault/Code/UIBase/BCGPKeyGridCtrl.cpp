#include "StdAfx.h"
#include "BCGPKeyGridCtrl.h"

CBCGPKeyGridCtrl::CBCGPKeyGridCtrl(void)
{
	m_bEnalbeKeyMove = TRUE;
}

CBCGPKeyGridCtrl::~CBCGPKeyGridCtrl(void)
{
}

BOOL CBCGPKeyGridCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_bEnalbeKeyMove)
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_LEFT)
			{
				if (::GetKeyState(VK_CONTROL)&0x80)
				{
					return MoveItem(VK_LEFT);
				}
			}
			else if(pMsg->wParam == VK_RIGHT)
			{
				if (::GetKeyState(VK_CONTROL)&0x80)
				{
					return MoveItem(VK_RIGHT);
				}
			}
			else if(pMsg->wParam == VK_DOWN)
			{
				return MoveItem(VK_DOWN);
			}
			else if(pMsg->wParam == VK_UP)
			{
				return MoveItem(VK_UP);
			}
		}
	}
	
	return CBCGPGridCtrl::PreTranslateMessage(pMsg);
}

BOOL CBCGPKeyGridCtrl::MoveItem(UINT _uDirection)
{
	CBCGPGridRow* pCurSelRow = GetCurSel();
	if(pCurSelRow == NULL)
	{
		return TRUE;
	}
	CBCGPGridItem* pCurSelItem = GetCurSelItem(pCurSelRow);
	if(pCurSelItem == NULL)
	{
		return TRUE;
	}
	if(pCurSelItem->GetInPlaceWnd() != NULL)
	{
		pCurSelItem->OnUpdateValue();
	}
	int iRow = -1;
	for(int i = 0; i < GetRowCount();i++)
	{
		CBCGPGridRow* pItrRow = GetRow(i);
		if(pItrRow == pCurSelRow)
		{
			iRow = i;
			break;
		}
	}
	if(iRow == -1)
	{
		return TRUE;
	}
	int iCol = -1;
	for(int i = 0;i < pCurSelRow->GetItemCount();i++)
	{
		CBCGPGridItem* pItrItem = pCurSelRow->GetItem(i);
		if(pItrItem == pCurSelItem)
		{
			iCol = i;
			break;
		}
	}
	if(iCol == -1)
	{
		return TRUE;
	}
	if(_uDirection == VK_LEFT)
	{
		if(iCol <= 0)
		{
			return TRUE;
		}
		iCol--;
	}
	else if(_uDirection == VK_RIGHT)
	{
		if(iCol >= GetColumnCount() -1)
		{
			return TRUE;
		}
		iCol++;
	}
	else if(_uDirection == VK_UP)
	{
		if(iRow <= 0)
		{
			return TRUE;
		}
		iRow--;
	}
	else if(_uDirection == VK_DOWN)
	{
		if(iRow >= GetRowCount() -1)
		{
			return TRUE;
		}
		iRow++;
	}
	else
	{
		ASSERT(FALSE);
	}
	CBCGPGridRow* pNextSelRow = GetRow(iRow);//要新选择的Item
	if(pNextSelRow == NULL)
	{
		return TRUE;
	}
	CBCGPGridItem* pNextItem = pNextSelRow->GetItem(iCol);
	if(pNextItem == NULL)
	{
		return TRUE;
	}
	SetCurSel(pNextItem->GetGridItemID());
	return TRUE;
}