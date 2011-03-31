#pragma once
/******************************************************
特别处理RQ的数据基类说明
RQ可以使用的数据类型有以下几中：
(如果要修改数据表请修改对应派生类的构造函数方法)

数据库类型    ADO代码         大小     C++对象
int           adInteger       4        DWORD
char          adChar          ..       CString  
datetime      adDBTimeStamp   ..       COleDateTime
float         adDouble        ..       double

在调用Close方法时，请先设置做如下操作，否则会异常
SetFilter("");
MoveFirst();
*****************************************************/
#include "AdoRecordSet.h"
class  CRQDb  : public CAdoRecordSet
{
protected:
	CRQDb();
	~CRQDb();
public:
	BOOL            RunSql(CString sql);
	void            Close();
	BOOL            Open(CString table,CAdoConnection *connect);
	BOOL            Update(DWORD id);
	BOOL            Load(CString sql);
	BOOL            Load(DWORD id);	
	BOOL            Delete(DWORD id);                  
	BOOL            Add(); 
	BOOL            Move(BOOL isNext = TRUE); 
	BOOL            Commit();              
protected:
	CString         m_table;
	CAdoConnection *m_connect;
    CMapStringToPtr *m_colMap;
protected:
	BOOL            SetQuery( CString  sql);           //设置查询的过滤参数			  
private:
	BOOL            UpdateRecord(BOOL bLoad);              //记录集合和成员的数据交换 
	BOOL            UpdateColData(BOOL bLoad,
		                          CString colName,
							      void *data);
};
