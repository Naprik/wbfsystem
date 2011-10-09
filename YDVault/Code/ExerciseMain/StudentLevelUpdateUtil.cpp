#include "StdAfx.h"
#include "StudentLevelUpdateUtil.h"
#include "../Base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDQuestionVault.h"
#include "ExerciseMain.h"
CStudentLevelUpdateUtil	CStudentLevelUpdateUtil::s_instance;
CStudentLevelUpdateUtil::CStudentLevelUpdateUtil(void)
{
}


CStudentLevelUpdateUtil::~CStudentLevelUpdateUtil(void)
{
	for (auto itr=m_vault2level.begin(); itr != m_vault2level.end();
			++itr)
	{
		CListAutoClean<CYDObjectRef> clr(itr->second);
	}
}

HRESULT CStudentLevelUpdateUtil::GetStudentLevel(OBJID vaultid, 
													int condition,
													CString* plevel)
{
	HRESULT hr = E_FAIL;
	if (m_vault2level.size() <= 0)
	{
		hr = Init();
		if (FAILED(hr))
		{
			return hr;
		}
	}

	auto itrcfg = m_vault2level.find(vaultid);
	if (itrcfg != m_vault2level.end())
	{
		int findcondition = (std::numeric_limits<int>::min)();
		for (auto itr=itrcfg->second.begin(); itr != itrcfg->second.end(); ++itr)
		{
			long currentcondition = 0;
			hr = (*itr)->GetPropVal(FIELD_VAULTLEVEL_CONDITION, &currentcondition);
			if (FAILED(hr))
			{
				return hr;
			}
			if (condition >= currentcondition && findcondition < currentcondition)
			{
				findcondition = currentcondition;
				CString strLevel;
				hr = (*itr)->GetPropVal(FIELD_VAULTLEVEL_LEVEL, strLevel);
				if (FAILED(hr))
				{
					return hr;
				}
				CString strVault;
				CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
				CYDQuestionVault vault(pDB);
				vault.SetID(vaultid);
				vault.GetPropVal(L"name", strVault);
				*plevel = strVault + L":" + strLevel;
			}
				
		}
	}
	return S_OK;
}

HRESULT CStudentLevelUpdateUtil::Init()
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> cfgs;
	hr  = CStaticObjHelper::GetObjRef(DB_VAULTLEVEL, theApp.m_pDatabase, &cfgs);
	if (FAILED(hr))
	{
		return hr;
	}

	for (auto itr = cfgs.begin(); itr != cfgs.end(); ++itr)
	{
		AddCfg(*itr);
	}
	return S_OK;
}

HRESULT CStudentLevelUpdateUtil::AddCfg(CYDObjectRef* cfg)
{
	HRESULT hr = E_FAIL;
	long vaultid;
	hr = cfg->GetPropVal(FIELD_VAULTLEVEL_VAULTID, &vaultid);
	if (FAILED(hr))
	{
		return hr;
	}
	auto itr = m_vault2level.find(vaultid);
	if (itr != m_vault2level.end())
	{
		itr->second.push_back(cfg);
	}
	else
	{
		std::list<CYDObjectRef*> cfgs;
		cfgs.push_back(cfg);
		m_vault2level.insert(MapLevel::value_type(vaultid, cfgs));
	}

	return S_OK;
}
