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
	//���������õ���Ӧ�Ķ�����Ϣ
	static HRESULT GetObjRef(
		CString _strDBName,
		CDatabaseEx* pDB,
		std::list<CYDObjectRef*>* _lstref,
		std::list<CPropQueryContidition*>* _lstQueryCondition = NULL);
	//��_pSourceObj����һ���µĶ���_pNewObj�����ǵ�Id��DBName����ͬ
	static HRESULT CopyObj(CYDObjectRef* _pSourceObj,
							CDatabaseEx* pDB,
							CYDObjectRef* &_pNewObj);
private:
	static HRESULT CreateObjRefByDBName(CString _strDBName,
										CDatabaseEx* pDB,
										CYDObjectRef* &_pRef);

};
