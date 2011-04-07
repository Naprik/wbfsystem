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
	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
	//Ìâ¿â
	OBJID idVault = 0;
	hr = _pVault->GetID(&idVault);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIDVault;
	strIDVault.Format(_T("%d"),idVault);
	CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDFACTORINFOITEM_VAULTID;
	pPropQueryContidition->m_strConVal = strIDVault;
	lstQueryCon.push_back(pPropQueryContidition);
	//_pQType
	OBJID idType = 0;
	hr = _pQType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIDType;
	strIDType.Format(_T("%d"),idType);
	pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDFACTORINFOITEM_QTYPEID;
	pPropQueryContidition->m_strConVal = strIDType;
	lstQueryCon.push_back(pPropQueryContidition);

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDFACTORINFOITEM,pDB,_pListFactorInfo,&lstQueryCon);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
