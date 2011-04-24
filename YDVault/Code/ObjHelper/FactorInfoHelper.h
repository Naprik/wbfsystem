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
	//�õ� ��ǰ��⵱ǰ���͡�Ԥ���ֶΡ���ָ���ӳ���ϵ�����б�
	HRESULT GetFactorInfoByVaultQType(CDatabaseEx* pDb,CYDObjectRef* _pVault,CYDObjectRef* _pQType,
										std::list<CYDObjectRef*> *_pListFactorInfo);
	HRESULT GetFactorInfoByVaultQType(CDatabaseEx* pDb,OBJID _idVault,OBJID _idQType,
										std::list<CYDObjectRef*> *_pListFactorInfo);
	//�ж�_strFieldName�Ƿ�Ϊ��ֵ��
	BOOL	IsNumberFieldName(CString _strFieldName);
};

