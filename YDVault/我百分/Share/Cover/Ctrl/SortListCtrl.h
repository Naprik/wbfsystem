#ifndef SORTLISTCTRL_H
#define SORTLISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ListCtrlEx.h"

#ifndef SORTHEADERCTRL_H
	#include "SortHeaderCtrl.h"
#endif	// SORTHEADERCTRL_H

#ifdef _DEBUG
	#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
	#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG

class CSortListCtrl : public CListCtrlEx
{
	struct ItemData
	{
	public:
		ItemData() : arrpsz( NULL ), dwData( NULL ) {}
        LPTSTR* arrpsz;
     	DWORD dwData;
    private:
		// ban copying.
		ItemData( const ItemData& );
		ItemData& operator=( const ItemData& );
	};
	DECLARE_DYNCREATE(CSortListCtrl)
public:
	CSortListCtrl();
private:
	static bool IsPercent(LPCTSTR pszText);
	static int  PercentCompare( LPCTSTR strPer1, LPCTSTR strPer2 );
	static bool IsNumber( LPCTSTR pszText );
	static int  NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 );
	static bool IsDate( LPCTSTR pszText );
	static int  DateCompare( const CString& strDate1, const CString& strDate2 );
public:
	BOOL ClearHeadings( );
	BOOL SetHeadings( UINT uiStringID );
	BOOL SetHeadings( const CString& strHeadings );
	int AddItem( int pic,LPCTSTR pszText, ... );
	BOOL DeleteItem( int iItem );
	BOOL DeleteAllItems();
	void LoadColumnInfo();
	void SaveColumnInfo();
	BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
	void Sort( int iColumn, BOOL bAscending );
	BOOL SetItemData(int nItem, DWORD dwData);
	DWORD GetItemData(int nItem) const;
	//{{AFX_VIRTUAL(CSortListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
public:
	CString GetColumnText(int col);
	int GetColumnAccount();
	BOOL IsSelect(int item);
	void SetFulSelect();
	void SetCheckSelect();
	void DeSelectAll();
	void SelectAll( );
	virtual ~CSortListCtrl();
protected:
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
	void FreeItemMemory( const int iItem );
	BOOL CSortListCtrl::SetTextArray( int iItem, LPTSTR* arrpsz );
	LPTSTR* CSortListCtrl::GetTextArray( int iItem ) const;
	CSortHeaderCtrl m_ctlHeader;
	int m_iNumColumns;
	int m_iSortColumn;
	BOOL m_bSortAscending;
	//{{AFX_MSG(CSortListCtrl)
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
#endif // SORTLISTCTRL_H
