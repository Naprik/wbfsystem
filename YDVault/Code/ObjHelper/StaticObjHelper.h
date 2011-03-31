#pragma once

#include <list>
class CDatabaseEx;
class CYDObjectRef;
class CPropQueryContidition;
class BASE_DLL_API CStaticObjHelper
{
public: 
	CStaticObjHelper(void);
	~CStaticObjHelper(void);
public:
	//根据条件得到对应的对象信息
	static HRESULT GetObjRef(
		CString _strDBName,
		CDatabaseEx* pDB,
		std::list<CYDObjectRef*>* _lstref,
		std::list<CPropQueryContidition*>* _lstQueryCondition = NULL);
	//从_pSourceObj复制一个新的对象_pNewObj，他们的Id和DBName都相同
	static HRESULT CopyObj(CYDObjectRef* _pSourceObj,
							CDatabaseEx* pDB,
							CYDObjectRef* &_pNewObj);
private:
	static HRESULT CreateObjRefByDBName(CString _strDBName,
										CDatabaseEx* pDB,
										CYDObjectRef* &_pRef);

};
