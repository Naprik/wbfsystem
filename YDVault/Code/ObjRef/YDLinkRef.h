/*
* Copyright (c) 2010, 南京永东
* All rights reserved.
*
* 文件名称：YDLinkRef.h
* 摘 要：所有YDVault中关系的基类
*
* 当前版本：1.0
* 作 者：wym
* 完成日期：2010年4月20日
*/
#pragma once
#include "YDObjectRef.h"
class CDatabaseEx;


class BASE_DLL_API CYDLinkRef 
	: public CYDObjectRef
{
public:
	CYDLinkRef(CDatabaseEx* pDB,CString _strDBNaem);
	~CYDLinkRef(void);
public:
	virtual HRESULT Update();
	virtual HRESULT Save();
	virtual HRESULT PutObjRef(CYDObjectRef* _pObjA,CYDObjectRef* _pObjB);
	virtual HRESULT GetObjRef(CYDObjectRef* &_pObjA,CYDObjectRef* &_pObjB);
	virtual HRESULT LoadByObj();

protected:
	virtual HRESULT OracleSave();
	virtual HRESULT SQLServerSave();
	virtual HRESULT AccessSave();


private:
	CYDObjectRef*       m_pObjRefA;
	CYDObjectRef*       m_pObjRefB;
public:
protected:
	virtual HRESULT FillObjIDParamDB();
private:
};
