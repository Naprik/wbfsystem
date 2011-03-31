// MyList.cpp : implementation file
//

#include "stdafx.h"
//#include "listctrl.h"
#include "MyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyList

CMyList::CMyList()
{

}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
	//{{AFX_MSG_MAP(CMyList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()

void CMyList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
// TODO: Add your control notification handler code here
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	
	switch(lplvdr->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
			
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
			
	case CDDS_ITEMPREPAINT:
		{
			COLORREF ItemColor;
			if( MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
			{
				lplvdr->clrText = ItemColor;
				*pResult = CDRF_DODEFAULT;
			}
		}
		break;


	}

}

void CMyList::SetItemColor(DWORD iItem, COLORREF color)
{
	//	m_iCurrentItem = iItem;
	//	m_CurrentColor = color;
	
	MapItemColor.SetAt(iItem, color);
	this->RedrawItems(iItem, iItem);
	UpdateWindow();
	
}

/////////////////////////////////////////////////////////////////////////////
// CMyList message handlers
