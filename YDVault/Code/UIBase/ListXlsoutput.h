#pragma once

//将listCtrl中数据导出到Excel中
#include "excel9.h"
#include <list>
class AFX_EXT_CLASS CListXlsoutput
{
public:
	CListXlsoutput(CListCtrl* _pList);
	~CListXlsoutput(void);
	HRESULT OutputExcel(BOOL _bOpen = TRUE);//将m_pListCtrl中数据导出到excel中
private:
	CListCtrl*	m_pListCtrl;
	BOOL		m_bOutExcel;
	HRESULT		InitExcel();//初始化Excel
	_ExcelApplication m_ExcelApp;
	Workbooks m_wbsMyBooks;
	//输出列表栏头信息
	HRESULT     OutputColumInfo(_Worksheet& _wsMysheet);
	//输出列表内容信息
	HRESULT     OutputListContentInfo(_Worksheet& _wsMysheet);
	//根据_index得到在Excel中的列，如果_index = 0,则_strCol = “A”,以此类推
	HRESULT     GetColumnNameByIndex(int _index,CString& _strCol);
	HRESULT     OutputCapitonInfo(_Worksheet& _wsMysheet);
public:
	std::list<std::pair<CString,CString> > m_lstCapiton;//相当于要输出的标题栏信息
};
