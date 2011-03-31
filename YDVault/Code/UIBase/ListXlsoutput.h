#pragma once

//��listCtrl�����ݵ�����Excel��
#include "excel9.h"
#include <list>
class AFX_EXT_CLASS CListXlsoutput
{
public:
	CListXlsoutput(CListCtrl* _pList);
	~CListXlsoutput(void);
	HRESULT OutputExcel(BOOL _bOpen = TRUE);//��m_pListCtrl�����ݵ�����excel��
private:
	CListCtrl*	m_pListCtrl;
	BOOL		m_bOutExcel;
	HRESULT		InitExcel();//��ʼ��Excel
	_ExcelApplication m_ExcelApp;
	Workbooks m_wbsMyBooks;
	//����б���ͷ��Ϣ
	HRESULT     OutputColumInfo(_Worksheet& _wsMysheet);
	//����б�������Ϣ
	HRESULT     OutputListContentInfo(_Worksheet& _wsMysheet);
	//����_index�õ���Excel�е��У����_index = 0,��_strCol = ��A��,�Դ�����
	HRESULT     GetColumnNameByIndex(int _index,CString& _strCol);
	HRESULT     OutputCapitonInfo(_Worksheet& _wsMysheet);
public:
	std::list<std::pair<CString,CString> > m_lstCapiton;//�൱��Ҫ����ı�������Ϣ
};
