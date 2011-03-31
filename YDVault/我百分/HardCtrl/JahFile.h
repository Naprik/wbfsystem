#define JAH_FILE_SAVE_LOCK "0xbe8e2ce4, 0xdabx, 0x14d6, 0x2d, 0xd2, 0x1, 0xe0, 0x4c, 0x53, 0xf6, 0xe8"
/*************************************************************************
������    CJahFile
���ʱ�䣺2004-9-10
���ߣ�    л��ΰ
���ࣺ    ��
�����ࣺ  CJahObject
�ؼ��֣�  �ļ����洢����ȡ
����˵�������ļ����д�ȡ�����������������ȵ�
ʹ��˵����
ע��㣺  1�����ļ������ļ�һд�ķ�ʽ����ָ���Զ��Ƶ��ļ�β
          2���ļ��ڴ�ʱ���Զ��������ڹرյ�ʱ��Ž������
		  3����д�ļ�Ƶ��ʱ�򣬽��鲻Ҫʹ�ô���������ȫ�ֱ�����
*************************************************************************/
#pragma once
#include "jahobject.h"
//##ModelId=4141A18702A8

class CJahFile  
{
public:
	DWORD          GetLength();
	BOOL           Seek(long pos);
	BOOL           WriteString(CString str);
	CString        ReadString();
	static BOOL    Remove(CString fileName);
	static BOOL    WriteFile(CString file,CString text);
	static CString ReadFile(CString file);
	//##ModelId=4141A18702C0
	BOOL      Lock();                                   //����
	//##ModelId=4141A18702C1
	BOOL      UnLock();                                 //����
	//##ModelId=4141A18702C6 
	BOOL      IsLocked();                               //�Ƿ���
	
	//##ModelId=4142B5B003A4
	BOOL      Open(CString file,BOOL isRead = TRUE);    //���ļ������ļ�һд�ķ�ʽ����ָ���Զ��Ƶ��ļ�β
	//##ModelId=4142B5B003A3
	void      Close();                                  //�ر��ļ�
	//##ModelId=4141A18702BD
	void      SeekTo(BOOL isEnd = TRUE );               //�ƶ�ָ�뵽�ļ�ͷ���ļ�β

	//##ModelId=4142B5B00390
	int       Read(void *buf , int len);                //д����
	//##ModelId=4142B5B00386
	BOOL      ReadObj(CJahObject *obj);                 //������
	//##ModelId=4142B5B0037B
	BOOL      ReadStruct(void *buf,int len);            //���ṹ

	//##ModelId=4142B5B00370
	BOOL      Write(void *buf , int len);               //������
	//##ModelId=4142B5B00366
	BOOL      WriteObj(CJahObject &obj);                //д����
	//##ModelId=4142B5B00352
	BOOL      WriteStruct(void *buf , int len);         //д�ṹ

	//##ModelId=4141A18702BC
	CJahFile();
	//##ModelId=4141A18702B8
	~CJahFile();
private:
	//##ModelId=4141A18702B5
	CFile     m_file;
	//##ModelId=4141A18702B2
	HANDLE    m_handle;
};

