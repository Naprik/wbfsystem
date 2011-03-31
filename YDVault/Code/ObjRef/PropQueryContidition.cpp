#include "StdAfx.h"
#include "PropQueryContidition.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DatabaseEx.h"

CPropQueryContidition::CPropQueryContidition(void)
{
	m_strFiledName = _T("");
	m_uFieldType = VT_BSTR;
	m_uOpType = Q_EQUAL;
	m_strConVal = _T("");
}

CPropQueryContidition::~CPropQueryContidition(void)
{
}

HRESULT	CPropQueryContidition::CreateContidition(CString &_strContidition)
{
	HRESULT hr = E_FAIL;
	if(m_strFiledName.IsEmpty())
	{
		return S_OK;
	}
	_strContidition = _T(" ");
	
	_strContidition += m_strFiledName;
	
	if(m_uOpType == Q_EQUAL)
	{
		if(m_strConVal.IsEmpty())
		{
			_strContidition = _T(" (");
			_strContidition += m_strFiledName;
			_strContidition += _T(" IS NULL OR ");
			_strContidition += m_strFiledName;
			_strContidition += _T(" = '') ");
		}
		else
		{
			_strContidition += _T(" = ");
			_strContidition += _T(" ? ");
		}
	}
	else if(m_uOpType == Q_NOTEQUAL)
	{
		if(m_strConVal.IsEmpty())
		{
			_strContidition += _T(" IS NOT NULL ");
		}
		else
		{
			_strContidition += _T(" <> ");
			_strContidition +=  _T(" ? ");
		}
	}
	else if(m_uOpType == Q_CONTAIN)
	{
		if(m_strConVal.IsEmpty())
		{
			_strContidition = _T("");
			return S_OK;
		}
		_strContidition += _T(" like ? ");
	}
	else if(m_uOpType == Q_IN)
	{
		_strContidition += _T(" IN ( ");
 		_strContidition += m_strConVal;
 		_strContidition += _T(" ) ");
	}
	else if(m_uOpType == Q_NOTIN)
	{
		_strContidition += _T(" NOT IN ( ");
 		_strContidition += m_strConVal;
 		_strContidition += _T(" ) ");
	}
	else if(m_uOpType == Q_UNKNOW)
	{
		_strContidition = _T("");
	}
	else 
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CPropQueryContidition::CreateParamter(CDatabaseEx* _pDB)
{
	HRESULT hr = E_FAIL;
	if(m_strFiledName.IsEmpty())
	{
		return S_OK;
	}
	if(m_strConVal.IsEmpty())
	{
		return S_OK;
	}
	if(m_uOpType == Q_NOTIN ||m_uOpType == Q_IN)
	{
		return S_OK;
	}
	CString strConVal = m_strConVal;
	if(m_uOpType == Q_CONTAIN)
	{
		strConVal = _T("%") + strConVal + _T("%");
	}
	CComVariant valProp(strConVal);
	ADODB::DataTypeEnum DataType ;
	int size = 0;
	CDataHandler::VarTypeToDBType(&valProp,DataType,size);
	hr = _pDB->AddParameter(_bstr_t(m_strFiledName), DataType, adParamInput, 
		size, &valProp);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}