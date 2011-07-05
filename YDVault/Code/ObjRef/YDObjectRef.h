/*
* Copyright (c) 2010, 南京永东
* All rights reserved.
*
* 文件名称：YDObjectRef.h
* 摘 要：所有YDVault中对象的基类
*
* 当前版本：1.0
* 作 者：wym
* 完成日期：2010年4月20日
*/

#pragma once

#include <list>
#include <map>
#include <comutil.h>
#include "../Include/CommonCompare.h"
#include "PropQueryContidition.h"

class CDatabaseEx;
class CXmlNode;
class CXmlWriter;

class BASE_DLL_API CYDObjectRef : public CObject
{
public:
	CYDObjectRef(CDatabaseEx* pDB);
	~CYDObjectRef(void);

public:
	HRESULT SetID(UINT ID);
	HRESULT GetID(UINT* _id);

	virtual HRESULT Restore(BOOL bReload = FALSE);
	virtual HRESULT Update();
	virtual HRESULT Save();
	virtual HRESULT Remove();
	
	HRESULT GetPropVal(BSTR _bstrPropName,VARIANT* _val);
	HRESULT GetPropVal(BSTR _bstrPropName,CString &_bstrVal);
	HRESULT GetPropVal(BSTR _bstrPropName,long* _pVal);
	HRESULT SetPropVal(BSTR _bstrPropName,VARIANT* _val);
	HRESULT GetBlobProp(BSTR _bstrPropName, BSTR _bstrPath);
	HRESULT UpdateBlob();
	HRESULT GetDBName(CString& _strDBName);
	HRESULT GetPropCount(UINT* _count);
	HRESULT GetProp(UINT i,BSTR* _bstrName,VARIANT* _val);
	HRESULT GetPropDef(BSTR _bstrPropName,CYDPropDef*& _pPropDef);
	HRESULT AddPropDef(CString strPropName,UINT _type,CString _strDefault = _T(""));
	HRESULT DelPropDef(CString strPropName);

	virtual HRESULT GetLabel(CString* pStr);
	virtual BOOL IsEqual(CYDObjectRef* pobj);


	HRESULT IsFieldExist(const CString& strField, const CString& strVaule, BOOL* pbExist);

	HRESULT GetObjectByCondition(int _iPage,
									std::list<CPropQueryContidition*> *_lstCondition,
									std::list<CYDObjectRef*>* _lstObjects);
	virtual HRESULT GetObjectTotalNumber(std::list<CPropQueryContidition*> *_lstCondition,
		long* _lCount);
	virtual HRESULT LoadXml(CXmlNode& _node);
	virtual HRESULT SaveXml(CXmlWriter& _writer);
	
protected:
	virtual HRESULT OracleSave();
	virtual HRESULT SQLServerSave();
	virtual HRESULT AccessSave();
	//当有BLOB_OBJECT字段时(如存储图像字段)在新增的时候会调用该函数
	virtual HRESULT AddNewRecordByBlobOject();
	//判断是否有图像字段
	virtual HRESULT IsHasBlobObject(VARIANT_BOOL* _bHas);
	//不同的数据库，在保存的时候，得到m_uObjID方式不同
	virtual HRESULT OracleSaveID();
	virtual HRESULT SQLServerSaveID();
	virtual HRESULT AccessSaveID();

	virtual HRESULT OracleUpdate();
	virtual HRESULT SQLServerUpdate();
	virtual HRESULT AccessUpdate();

	virtual HRESULT GetCurRecord(_RecordsetPtr& _pRecord);

protected:
	virtual CYDObjectRef* CreateObject();

	HRESULT AccessQueryCondition( std::list<CPropQueryContidition*> *_lstCondition);
	virtual HRESULT CreateSQLFromQueryCondition(std::list<CPropQueryContidition*> *_lstCondition, CString &_strSQL);
	HRESULT AccessExeCreateObject(std::list<CYDObjectRef*>* _lstObjects,
							int _iStart,
							int _iEnd);
	HRESULT OracleQueryCondition(int _iPage,std::list<CPropQueryContidition*> *_lstCondition);
	HRESULT OracleExeCreateObject(std::list<CYDObjectRef*>* _lstObjects);

	virtual	HRESULT CreateParamter(std::list<CPropQueryContidition*> *_lstCondition);
public:
	CDatabaseEx*						m_pDb; 
	CString								m_strDBName;
protected:
	UINT								m_uObjID;
	
	typedef std::map<CString,_variant_t, CStringCompareNoCase> MapProp;
	MapProp								m_mapProp;
	std::list<CYDPropDef*>				m_lstPropDef;
protected:
	HRESULT FillParamDB();//将m_lstPropDef中的参数添加到数据库中
	void UseFieldDefault(CYDPropDef* pPropDef,VARIANT& var,ADODB::DataTypeEnum& type,int& size);
	virtual HRESULT FillIDParamDB();
	HRESULT LoadProperty(CXmlNode& _node);
};
