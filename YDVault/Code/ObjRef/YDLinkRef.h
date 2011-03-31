/*
* Copyright (c) 2010, �Ͼ�����
* All rights reserved.
*
* �ļ����ƣ�YDLinkRef.h
* ժ Ҫ������YDVault�й�ϵ�Ļ���
*
* ��ǰ�汾��1.0
* �� �ߣ�wym
* ������ڣ�2010��4��20��
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
