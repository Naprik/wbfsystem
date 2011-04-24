#pragma once
#include <list>
class CDatabaseEx;
class CYDObjectRef;
class BASE_DLL_API CFactorInfoHelper
{
public:
	CFactorInfoHelper(void);
	~CFactorInfoHelper(void);
public:
	//得到 当前题库当前题型“预留字段”与指标的映射关系对象列表
	HRESULT GetFactorInfoByVaultQType(CDatabaseEx* pDb,CYDObjectRef* _pVault,CYDObjectRef* _pQType,
										std::list<CYDObjectRef*> *_pListFactorInfo);
	HRESULT GetFactorInfoByVaultQType(CDatabaseEx* pDb,OBJID _idVault,OBJID _idQType,
										std::list<CYDObjectRef*> *_pListFactorInfo);
	//判断_strFieldName是否为数值型
	BOOL	IsNumberFieldName(CString _strFieldName);
};

