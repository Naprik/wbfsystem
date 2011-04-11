#include "StdAfx.h"
#include "FactorInfoHelper.h"
#include "../ObjRef/PropQueryContidition.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjHelper/StaticObjHelper.h"

CFactorInfoHelper::CFactorInfoHelper(void)
{
}


CFactorInfoHelper::~CFactorInfoHelper(void)
{
}

HRESULT CFactorInfoHelper::GetFactorInfoByVaultQType(CDatabaseEx* pDb,CYDObjectRef* _pVault,CYDObjectRef* _pQType,
										std::list<CYDObjectRef*> *_pListFactorInfo)
{
	HRESULT hr = E_FAIL;
	ASSERT(pDb);
	ASSERT(_pVault);
	ASSERT(_pQType);
	ASSERT(_pListFactorInfo);

	//Ìâ¿â
	OBJID idVault = 0;
	hr = _pVault->GetID(&idVault);
	if(FAILED(hr))
	{
		return hr;
	}
	
	//_pQType
	OBJID idType = 0;
	hr = _pQType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr;
	}
	return GetFactorInfoByVaultQType(pDb,idVault,idType,_pListFactorInfo);
	return S_OK;
}

HRESULT CFactorInfoHelper::GetFactorInfoByVaultQType(CDatabaseEx* pDb,OBJID _idVault,OBJID _idQType,
										std::list<CYDObjectRef*> *_pListFactorInfo)
{
	HRESULT hr = E_FAIL;
	ASSERT(pDb);
	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
	CString strIDVault;
	strIDVault.Format(_T("%d"),_idVault);
	CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDFACTORINFOITEM_VAULTID;
	pPropQueryContidition->m_strConVal = strIDVault;
	lstQueryCon.push_back(pPropQueryContidition);
	CString strIDType;
	strIDType.Format(_T("%d"),_idQType);
	pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDFACTORINFOITEM_QTYPEID;
	pPropQueryContidition->m_strConVal = strIDType;
	lstQueryCon.push_back(pPropQueryContidition);
	hr = CStaticObjHelper::GetObjRef(DB_YDFACTORINFOITEM,pDb,_pListFactorInfo,&lstQueryCon);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

BOOL CFactorInfoHelper::IsNumberFieldName(CString _strFieldName)
{
	for(int iNumber = 1; iNumber <= 25;iNumber++)
	{
		CString strNumber;
		strNumber.Format(_T("D%d"),iNumber);
		if(_strFieldName.CompareNoCase(strNumber) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}