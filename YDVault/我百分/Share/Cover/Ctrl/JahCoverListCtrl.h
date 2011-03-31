
#pragma once
#include <afxtempl.h>
//
/////////////////////////////////////////////////////////////////////////////
// CJahCoverListCtrl window
#define         JAH_CTRL_STYLE_ICON           0
#define         JAH_CTRL_STYLE_SMALLICON      1
#define         JAH_CTRL_STYLE_LIST           2
#define         JAH_CTRL_STYLE_REPORT         3

//##ModelId=4140A36702CA
class CJahCoverListCtrl : public CListCtrl 
{
public:
	//##ModelId=4140A36702F9
	CJahCoverListCtrl();
public:
	//##ModelId=4140A36702D5
	HICON        m_hIcon;
	CImageList	 m_cImageListNormal, m_cImageListSmall, m_cImageListState;
public:
	//{{AFX_VIRTUAL(CJahCoverListCtrl)
	//}}AFX_VIRTUAL
public:
	void SetGrid();
	//##ModelId=4140A36702F8
	CString GetCtrlListCtrl();
	//##ModelId=4140A36702F6
	void SetCtrlListCtrl(char *buf);
	//##ModelId=4140A36702F5
	void DeSelectAll();
	//##ModelId=4140A36702F3
	void SelectAll(BOOL flag = TRUE);
	//##ModelId=4140A36702F1
	BOOL IsSelect( int item );
	//##ModelId=4140A36702EE
	void SelectRow(int item , BOOL flag = TRUE );
	//##ModelId=4140A36702EB
	//##ModelId=4140A36702EA
	void SetCheckSelect();
	//##ModelId=4140A36702E9
	void SetFulSelect();
	//##ModelId=4140A36702E6
	void SetStyle( int style = JAH_CTRL_STYLE_REPORT);
	//##ModelId=4140A36702E4
	virtual ~CJahCoverListCtrl();
protected:
	//{{AFX_MSG(CJahCoverListCtrl)
	//##ModelId=4140A36702E0
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//##ModelId=4140A36702DE
	CString GetCtrlList(CArray<int,int> *ctrlList);
	//##ModelId=4140A36702D9
	void SetCtrlList(char *buf,	CArray<int,int> *ctrlList);
	//##ModelId=4140A36702D6
    void Sort( int iColumn, BOOL bAscending );
};

