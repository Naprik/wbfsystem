#pragma once
//��ȡExcel�ļ����ݵ�lst��
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
	HRESULT		InitExcel();//��ʼ��Excel
	_ExcelApplication m_ExcelApp;
	Workbooks m_wbsMyBooks;
	//��ȡ�б���ͷ��Ϣ
	HRESULT     InputColumInfo(_Worksheet& _wsMysheet);
	//��ȡ�б�������Ϣ
	HRESULT     InputListContentInfo(_Worksheet& _wsMysheet,
									std::list<std::list<std::pair<CString,CString> > > *_pList);
	std::list<CString> m_lstColumName;
};
