#include "StdAfx.h"
#include "BCGPSortListCtrl.h"
#include "../Base\AutoClean.h"

IMPLEMENT_DYNAMIC(CBCGPSortListCtrl, CBCGPListCtrl)

CBCGPSortListCtrl::CBCGPSortListCtrl(void)
{
}

CBCGPSortListCtrl::~CBCGPSortListCtrl(void)
{
	CListAutoClean<CMyListItem> clr(m_lstItem);
}

BEGIN_MESSAGE_MAP(CBCGPSortListCtrl, CBCGPListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL IsNumber(const CString& str)
{
	if(str.IsEmpty()) return FALSE;
	short iD = 0;
	int nLen = str.GetLength();
	for(int i=0;i<nLen;i++)
	{
		if(str[i] < _T('0') || str[i] > _T('9')) //如果不是数字
		{
			if(str[i] == _T('-'))
			{
				if(i != 0) return FALSE;
			}
			else if(str[i] == _T('.'))
			{
				iD++;
				if(iD > 1) return FALSE;
			}
			else return FALSE;
		}
	}
	return TRUE;
}

int CBCGPSortListCtrl::OnCompareItems (LPARAM lParam1, LPARAM lParam2, int iColumn)
{
	CObject* pObj1 = (CObject*)lParam1;
	CObject* pObj2 = (CObject*)lParam2;

	int nItem1 = -1;
	int nItem2 = -1;
	for(int i=0; i<GetItemCount(); i++)
	{
		CObject* pObj = (CObject*)GetItemData(i);
		if (pObj == pObj1) 
		{
			nItem1 = i;
		}
		else if (pObj == pObj2) 
		{
			nItem2 = i;
		}
		if ( (nItem1 != -1) && (nItem2 != -1) )
		{
			break;
		}
	}
	if ( (nItem1 == -1) || (nItem2 == -1) )
	{
		return 0;
	}
	CString str1 = GetItemText (nItem1, iColumn);
	CString str2 = GetItemText (nItem2, iColumn);
	if(str1.Right(1).CompareNoCase(_T("\\%")) == 0)
	{
		str1 = str1.Left(str1.GetLength() - 1);
	}
	if(str2.Right(1).CompareNoCase(_T("\\%")) == 0)
	{
		str2 = str2.Left(str2.GetLength() - 1);
	}

	int nRes = 0;
	if(IsNumber(str1) && IsNumber(str2))
	{
		TCHAR* stop = NULL;
		double f1 = _tcstod((LPCTSTR)str1,&stop);
		double f2 = _tcstod((LPCTSTR)str2,&stop);
		if(f1 > f2) nRes = 1;
		else if(f1 < f2) nRes = -1;
	}
	else
	{
		nRes = lstrcmp(str1, str2);
	}
	return nRes;
}

COLORREF CBCGPSortListCtrl::OnGetCellTextColor (int nRow, int nColumn)
{
	for(std::list<CMyListItem*>::const_iterator itr = m_lstItem.begin();
		itr != m_lstItem.end();++itr)
	{
		if((*itr)->m_iRow == nRow && (*itr)->m_iCol == nColumn)
		{
			if((*itr)->m_clrText != -1)
			{
				return (*itr)->m_clrText;
			}
			break;
		}
	}

	return CBCGPListCtrl::OnGetCellTextColor (nRow, nColumn);
}

COLORREF CBCGPSortListCtrl::OnGetCellBkColor (int nRow, int nColumn)
{
	for(std::list<CMyListItem*>::const_iterator itr = m_lstItem.begin();
		itr != m_lstItem.end();++itr)
	{
		if((*itr)->m_iRow == nRow && (*itr)->m_iCol == nColumn)
		{
			if((*itr)->m_clrBckg != -1)
			{
				return (*itr)->m_clrBckg;
			}
			break;
		}
	}
	return CBCGPListCtrl::OnGetCellBkColor (nRow, nColumn);
}

/********************************************
函数名：SetItemColor
参数：CBCGPSortListCtrl *lsList
	  nColumns 设置的列
	  nRow     设置的行
说明：设置List中的颜色
	  nColumns、nRow从0开始设置
********************************************/

void CBCGPSortListCtrl::SetItemColor(int nRow,int nColumn,
									 COLORREF _colText/* = ITEM_TEXT_COLOR*/,
									 COLORREF _colBk /*= ITEM_TEXT_BK_COLOR*/,
									 BOOL _bInvalidata /* = True*/)
{
	CMyListItem* pItem = NULL;

	for(std::list<CMyListItem*>::const_iterator itr = m_lstItem.begin();
		itr != m_lstItem.end();++itr)
	{
		if((*itr)->m_iRow == nRow && (*itr)->m_iCol == nColumn)
		{
			pItem = (*itr);
			break;
		}
	}
	if (pItem == NULL)
	{
		pItem = new CMyListItem (nRow,nColumn , _colText, _colBk);
		m_lstItem.push_back(pItem);
		DWORD_PTR pData = (DWORD_PTR)GetItemData(nRow);
		pItem->m_pData = pData;
	}
	else
	{
		pItem->m_clrText = _colText;
		pItem->m_clrBckg = _colBk;
	}
	if(_bInvalidata)
	{
		RedrawWindow ();
	}
}

void CBCGPSortListCtrl::Sort (int iColumn, BOOL bAscending /*= TRUE*/, BOOL bAdd /*= FALSE*/)
{
	CBCGPListCtrl::Sort(iColumn,bAscending,bAdd);
	for(std::list<CMyListItem*>::const_iterator itr = m_lstItem.begin();
		itr != m_lstItem.end();++itr)
	{
		for(int i = 0; i < GetItemCount();i++)
		{
			DWORD_PTR pData = (DWORD_PTR)GetItemData(i);
			if(pData == (*itr)->m_pData)
			{
				(*itr)->m_iRow = i;
				break;
			}
		}
	}
}

void CBCGPSortListCtrl::SetRowColor(int nRow,COLORREF _colText /*= ITEM_TEXT_COLOR*/,
										COLORREF _colBk /*= ITEM_TEXT_BK_COLOR*/,
										BOOL _bInvalidata /* = TRUE*/)
{
	for(int i = 0; i < GetHeaderCtrl().GetItemCount();i++)
	{
		SetItemColor(nRow,i,_colText,_colBk,_bInvalidata);
	}
}

void CBCGPSortListCtrl::SetSpaceRowColor(BOOL _bSpaceRowColor)
{
	if(_bSpaceRowColor)
	{
		for(int i = 0; i < GetItemCount();i++)
		{
			if(i % 2 == 0)
			{
				SetRowColor(i,ITEM_TEXT_COLOR,ITEM_TEXT_BK_COLOR,FALSE);
			}
			else
			{
				SetRowColor(i,ITEM_TEXT_COLOR_DEFAULT,ITEM_TEXT_BK_COLOR_DEFAULT,FALSE);
			}
		}
	}
	else
	{
		for(int i = 0; i < GetItemCount();i++)
		{
			SetRowColor(i,ITEM_TEXT_COLOR_DEFAULT,ITEM_TEXT_BK_COLOR_DEFAULT,FALSE);
		}
	}
	Invalidate(TRUE);
}