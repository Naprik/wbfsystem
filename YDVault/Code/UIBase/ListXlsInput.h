#pragma once
//读取Excel文件内容到lst中
#include "excel9.h"
#include <list>
class AFX_EXT_CLASS CListXlsInput
{
public:
	CListXlsInput(CString _strXlsFile);
	~CListXlsInput(void);
	HRESULT Read(std::list<std::list<std::pair<CString,CString> > > *_pList);
private:
	CString m_strXlsFile;
	BOOL		m_bOutExcel;
	HRESULT		InitExcel();//初始化Excel
	_ExcelApplication m_ExcelApp;
	Workbooks m_wbsMyBooks;
	//读取列表栏头信息
	HRESULT     InputColumInfo(_Worksheet& _wsMysheet);
	//读取列表内容信息
	HRESULT     InputListContentInfo(_Worksheet& _wsMysheet,
									std::list<std::list<std::pair<CString,CString> > > *_pList);
	std::list<CString> m_lstColumName;
};
