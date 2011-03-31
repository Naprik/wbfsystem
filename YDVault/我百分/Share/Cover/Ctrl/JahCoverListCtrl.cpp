#include "stdafx.h"
#include "JahCoverListCtrl.h"

//##ModelId=4140A36702F9
CJahCoverListCtrl::CJahCoverListCtrl()
{
}
//##ModelId=4140A36702E4
CJahCoverListCtrl::~CJahCoverListCtrl()
{
}

BEGIN_MESSAGE_MAP(CJahCoverListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CJahCoverListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//##ModelId=4140A36702E6
void CJahCoverListCtrl::SetStyle(int style)
{
	SetRedraw(FALSE);
	switch (style)
	{
	case JAH_CTRL_STYLE_ICON: // Icon mode
	    ModifyStyle(0, LVS_ICON);
		break;
	case JAH_CTRL_STYLE_SMALLICON:	// Small icons
		ModifyStyle(0, LVS_SMALLICON);
		break;
	case JAH_CTRL_STYLE_LIST: // List mode
		ModifyStyle(0, LVS_LIST);
		break;
	case JAH_CTRL_STYLE_REPORT:	// Details mode
		ModifyStyle(0, LVS_REPORT);
		break;
	default:
		break;
	}
	SetRedraw(TRUE);
}
//##ModelId=4140A36702E9
void CJahCoverListCtrl::SetFulSelect()
{
	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	SetExtendedStyle(dwStyle);
}
//##ModelId=4140A36702EA
void CJahCoverListCtrl::SetCheckSelect()
{
	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
	SetExtendedStyle(dwStyle);
}
//##ModelId=4140A36702EE
void CJahCoverListCtrl::SelectRow(int item , BOOL flag)
{	
	SetCheck(item,flag);
}
//##ModelId=4140A36702F1
BOOL CJahCoverListCtrl::IsSelect(int item)
{	
	return GetCheck(item);
}
//##ModelId=4140A36702E0
void CJahCoverListCtrl::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;
	static BOOL flag = FALSE;
	flag = !flag;
	Sort( iColumn, TRUE );
	*pResult = 0;
}

int CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2,LPARAM lParamData)
{
	CJahCoverListCtrl* pListCtrl = reinterpret_cast<CJahCoverListCtrl*>( lParamData );
    int ret = -1;
	if ( lParam1 > lParam2)
		ret = 1;
	if ( lParam1 == lParam2 )
		ret = 0;
	return ret;//结果为>0 =0 <0
}
//##ModelId=4140A36702D6
void CJahCoverListCtrl::Sort( int iColumn, BOOL bAscending )
{
	VERIFY( SortItems( CompareFunction , reinterpret_cast<DWORD>( this )));
}
//##ModelId=4140A36702F3
void CJahCoverListCtrl::SelectAll(BOOL flag)
{
	int account = GetItemCount();
	for ( int i = 0 ;i < account ;i ++)
		SelectRow( i , flag );
}
//##ModelId=4140A36702F5
void CJahCoverListCtrl::DeSelectAll()
{
	int account = GetItemCount();
	for ( int i = 0 ;i < account ;i ++)
		SelectRow( i , !IsSelect( i ) );
}

/////////////////////////////////////////////////////
//函数名：SetCtrlList                              //
//参数一：char *                                   //
//参数二：CArray *                                 //
//返回值：无                                       //
//说明  ：将char *中的字符串中的”,“，分离成动态  //
//        数组。设置控制列表。                     //
/////////////////////////////////////////////////////
//##ModelId=4140A36702D9
void CJahCoverListCtrl::SetCtrlList(char *buf,CArray<int,int> *ctrlList)
{
	ctrlList->RemoveAll();
	for(DWORD i =0 ;i <strlen(buf);i++)
	{
		char t[256];
		strcpy(t,buf);
		if ( t[i] == ',')
		{
			t[i] = '\0';
			strcpy(buf,buf+strlen(t)+1);
			i =0 ;
			ctrlList->Add(atoi(t));
		}
	}
}
/////////////////////////////////////////////////////
//函数名：GetCtrlList                              //
//参数  ：CArray *                                 //
//返回值：CString                                  //
//说明  ：将动态数组中的数据合并成字符串。         //
/////////////////////////////////////////////////////
//##ModelId=4140A36702DE
CString CJahCoverListCtrl::GetCtrlList(CArray<int,int> *ctrlList)
{
	CString time;
	for( int i =0 ; i < ctrlList->GetSize(); i ++)
	{
		CString temp;
		temp.Format("%d,",ctrlList->GetAt(i));
		time = time+temp;
	}
	return time;
}
/////////////////////////////////////////////////////
//函数名：SetCtrlListCtrl                          //
//参数一：char *                                   //
//参数二：CListCtrl *                              //
//返回值：无                                       //
//说明  ：将控制信息显示到列表中。                 //
/////////////////////////////////////////////////////
//##ModelId=4140A36702F6
void CJahCoverListCtrl::SetCtrlListCtrl(char *buf)
{
	CArray<int,int> ctrlList;
	SetCtrlList(buf ,&ctrlList );
	for( int i =0 ;i <GetItemCount(); i ++)
	{
		SetCheck( i , FALSE );
		for ( int j =0 ; j < ctrlList.GetSize() ; j ++ )
		{
			int k = atoi((char*)GetItemText(i , 0 ).GetBuffer(0));
			if ( k == ctrlList.GetAt( j ) )
			{
				SetCheck(i);
				break;
			}
		}
	}
}
/////////////////////////////////////////////////////
//函数名：GetCtrlListCtrl                          //
//参数  ：CListCtrl *                              //
//返回值：CString                                  //
//说明  ：从列表中取出控制信息。                   //
/////////////////////////////////////////////////////
//##ModelId=4140A36702F8
CString CJahCoverListCtrl::GetCtrlListCtrl()
{
	CArray<int,int> ctrlList;
	ctrlList.RemoveAll();
	for( int i =0 ; i < GetItemCount(); i ++)
	{
		if ( GetCheck( i ) )
		{
			int k = atoi((char*)GetItemText(i , 0 ).GetBuffer(0));
			ctrlList.Add(k);
		}
	}
	return GetCtrlList(&ctrlList);
}

void CJahCoverListCtrl::SetGrid()
{
	DWORD dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	SetExtendedStyle(dwStyle );
}
