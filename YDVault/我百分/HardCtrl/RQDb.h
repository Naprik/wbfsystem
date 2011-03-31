#pragma once
/******************************************************
�ر���RQ�����ݻ���˵��
RQ����ʹ�õ��������������¼��У�
(���Ҫ�޸����ݱ����޸Ķ�Ӧ������Ĺ��캯������)

���ݿ�����    ADO����         ��С     C++����
int           adInteger       4        DWORD
char          adChar          ..       CString  
datetime      adDBTimeStamp   ..       COleDateTime
float         adDouble        ..       double

�ڵ���Close����ʱ���������������²�����������쳣
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
	BOOL            SetQuery( CString  sql);           //���ò�ѯ�Ĺ��˲���			  
private:
	BOOL            UpdateRecord(BOOL bLoad);              //��¼���Ϻͳ�Ա�����ݽ��� 
	BOOL            UpdateColData(BOOL bLoad,
		                          CString colName,
							      void *data);
};
