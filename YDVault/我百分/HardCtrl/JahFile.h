#define JAH_FILE_SAVE_LOCK "0xbe8e2ce4, 0xdabx, 0x14d6, 0x2d, 0xd2, 0x1, 0xe0, 0x4c, 0x53, 0xf6, 0xe8"
/*************************************************************************
类名：    CJahFile
完成时间：2004-9-10
作者：    谢文伟
父类：    无
关联类：  CJahObject
关键字：  文件、存储、读取
功能说明：对文件进行存取操作，加锁，解锁等等
使用说明：
注意点：  1：打开文件，当文件一写的方式打开是指针自动移到文件尾
          2：文件在打开时候自动加锁，在关闭的时候才解除锁。
		  3：读写文件频繁时候，建议不要使用此内来创建全局变量。
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
	BOOL      Lock();                                   //加锁
	//##ModelId=4141A18702C1
	BOOL      UnLock();                                 //解锁
	//##ModelId=4141A18702C6 
	BOOL      IsLocked();                               //是否被锁
	
	//##ModelId=4142B5B003A4
	BOOL      Open(CString file,BOOL isRead = TRUE);    //打开文件，当文件一写的方式打开是指针自动移到文件尾
	//##ModelId=4142B5B003A3
	void      Close();                                  //关闭文件
	//##ModelId=4141A18702BD
	void      SeekTo(BOOL isEnd = TRUE );               //移动指针到文件头或文件尾

	//##ModelId=4142B5B00390
	int       Read(void *buf , int len);                //写数据
	//##ModelId=4142B5B00386
	BOOL      ReadObj(CJahObject *obj);                 //读对象
	//##ModelId=4142B5B0037B
	BOOL      ReadStruct(void *buf,int len);            //读结构

	//##ModelId=4142B5B00370
	BOOL      Write(void *buf , int len);               //读数据
	//##ModelId=4142B5B00366
	BOOL      WriteObj(CJahObject &obj);                //写对象
	//##ModelId=4142B5B00352
	BOOL      WriteStruct(void *buf , int len);         //写结构

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

