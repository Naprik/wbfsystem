#include "StdAfx.h"
#include "YDStuMark.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"

CYDStuMark::CYDStuMark(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDSTUMARK;
	AddPropDef(FIELD_YDSTUMARK_STUID, VT_I4);	
	AddPropDef(FIELD_YDSTUMARK_DEVICEID,VT_I4);	
	AddPropDef(FIELD_YDSTUMARK_EXAMINFOID,VT_I4);	
	AddPropDef(FIELD_YDSTUMARK_EXAMID,VT_BSTR);	
	AddPropDef(FIELD_YDSTUMARK_ANSWER,VT_BSTR);
	AddPropDef(FIELD_YDSTUMARK_PAPERID,VT_I4);
	AddPropDef(FIELD_YDSTUMARK_MARK,VT_R4);
	AddPropDef(FIELD_YDSTUMARK_SUBJECTID,VT_I4);
	AddPropDef(FIELD_YDSTUMARK_EXAMDATE,VT_DATE);
}

CYDStuMark::~CYDStuMark(void)
{
}

HRESULT CYDStuMark::GetStudent(CYdStudentRef*& pStu)
{
	pStu = new CYdStudentRef(m_pDb);
	long stuID;
	GetPropVal(FIELD_YDSTUMARK_STUID, &stuID);
	pStu->SetID(stuID);
	return S_OK;
}
HRESULT CYDStuMark::GetPaper(CYDPaper*& pPaper)
{
	pPaper = new CYDPaper(m_pDb);
	long paperID;
	GetPropVal(FIELD_YDSTUMARK_PAPERID, &paperID);
	pPaper->SetID(paperID);
	return S_OK;
}
HRESULT CYDStuMark::GetDevice(CYDDevice*& pDevice)
{
	pDevice = new CYDDevice(m_pDb);
	long DeviceID;
	GetPropVal(FIELD_YDSTUMARK_DEVICEID, &DeviceID);
	pDevice->SetID(DeviceID);
	return S_OK;
}

HRESULT CYDStuMark::GetSubject(CYDSubjectRef*& pSubject)
{
	pSubject = new CYDSubjectRef(m_pDb);
	long subjID;
	GetPropVal(FIELD_YDSTUMARK_SUBJECTID, &subjID);
	pSubject->SetID(subjID);
	return S_OK;
}

CYDObjectRef* CYDStuMark::CreateObject()
{
	return new CYDStuMark(m_pDb);
}

HRESULT CYDStuMark::GetCountBySelfProp(std::list<CPropQueryContidition*> *_lstCondition, 
											long* pCount)
{
	return GetObjectTotalNumber(_lstCondition, pCount);
}

HRESULT CYDStuMark::GetUnitsBySelfProp(int _iPage,
											std::list<CPropQueryContidition*> *_lstCondition, 
											std::list<CYDObjectRef*>* pListObjs)
{
	return GetObjectByCondition(_iPage, _lstCondition, pListObjs);
}

HRESULT CYDStuMark::GetCountByStu(std::list<CPropQueryContidition*> *_lstCondition, long* pCount)
{
	HRESULT hr = E_FAIL;
	CString strSQLUnit = _T("SELECT objid FROM ");
	strSQLUnit += m_strDBName;
	strSQLUnit += L" ";

	CString strSQLStu = FIELD_YDSTUMARK_STUID;
	strSQLStu += L" in ( select stu.objid from  ";
	strSQLStu += DB_YDSTUINFO;
	strSQLStu += L" stu ";
	BOOL bFirst = TRUE;
	BOOL bFirst1 = TRUE;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
		CString strItrSQL;

		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			strItrSQL += L" cdate(";
			strItrSQL += FIELD_YDSTUMARK_EXAMDATE;
			strItrSQL += L") ";
			if ((*itr)->m_uOpType == Q_GE)
			{
				strItrSQL += L">= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LE)
			{
				strItrSQL += L"<= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_GREATOR)
			{
				strItrSQL += L"> cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LOWER)
			{
				strItrSQL += L"< cdate('";
			}
			else
			{
				continue;
			}
			strItrSQL += (*itr)->m_strConVal;
			strItrSQL += L"') ";
		}
		else
		{
			hr = (*itr)->CreateContidition(strItrSQL);
			if (FAILED(hr))
			{
				return hr;
			}
			if(strItrSQL.IsEmpty())
			{
				continue;
			}
		}
		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_SUBJECTID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			if(bFirst1)
			{
				strSQLUnit += _T(" WHERE ");
				bFirst1 = FALSE;
			}
			else
			{
				strSQLUnit += _T(" AND ");
			}

			strSQLUnit += strItrSQL;
		}
		else
		{
			if(bFirst)
			{
				strSQLStu += _T(" WHERE ");
				bFirst = FALSE;
			}
			else
			{
				strSQLStu += _T(" AND ");
			}

			strSQLStu += strItrSQL;
		}
		
	}
	strSQLStu += L") ";
	CString strSQL = _T("Select Count(*) As IDCOUNT From ( ") + strSQLUnit + L" and " + strSQLStu;
	strSQL += _T(" ) ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	CreateParamter(_lstCondition);
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCount;
	hr = m_pDb->GetField(_variant_t(_T("IDCOUNT")),valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	*pCount = CDataHandler::VariantToLong(valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDStuMark::CreateParamter(std::list<CPropQueryContidition*> *_lstCondition)
{
	HRESULT hr = E_FAIL;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end();++itr)
	{
		if ((*itr)->m_uFieldType != VT_DATE &&
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) != 0)
		{
			hr = (*itr)->CreateParamter(m_pDb);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	return S_OK;
}

HRESULT CYDStuMark::CreateSQLFromQueryConditionByStu(std::list<CPropQueryContidition*> *_lstCondition,
														CString &_strSQL)
{
	HRESULT hr = E_FAIL;
	CString strSQLUnit = _T("SELECT objid FROM ");
	strSQLUnit += m_strDBName;

	CString strSQLStu = FIELD_YDSTUMARK_STUID;
	strSQLStu += L" in ( select stu.objid from  ";
	strSQLStu += DB_YDSTUINFO;
	strSQLStu += L" stu ";
	BOOL bFirst = TRUE;
	BOOL bFirst1 = TRUE;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
		CString strItrSQL;

		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			strItrSQL += L" cdate(";
			strItrSQL += FIELD_YDSTUMARK_EXAMDATE;
			strItrSQL += L") ";
			if ((*itr)->m_uOpType == Q_GE)
			{
				strItrSQL += L">= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LE)
			{
				strItrSQL += L"<= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_GREATOR)
			{
				strItrSQL += L"> cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LOWER)
			{
				strItrSQL += L"< cdate('";
			}
			else
			{
				continue;
			}
			strItrSQL += (*itr)->m_strConVal;
			strItrSQL += L"') ";
		}
		else
		{
			hr = (*itr)->CreateContidition(strItrSQL);
			if (FAILED(hr))
			{
				return hr;
			}
			if(strItrSQL.IsEmpty())
			{
				continue;
			}
			
		}
		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_SUBJECTID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			if(bFirst1)
			{
				strSQLUnit += _T(" WHERE ");
				bFirst1 = FALSE;
			}
			else
			{
				strSQLUnit += _T(" AND ");
			}

			strSQLUnit += strItrSQL;
		}
		else
		{
			if(bFirst)
			{
				strSQLStu += _T(" WHERE ");
				bFirst = FALSE;
			}
			else
			{
				strSQLStu += _T(" AND ");
			}

			strSQLStu += strItrSQL;
		}

	}
	strSQLStu += L") ";
	_strSQL = strSQLUnit + L" and " + strSQLStu + _T(" ORDER BY OBJID  ");
	
	return S_OK;
}

HRESULT CYDStuMark::CalMark(double* pMark)
{
	CYDPaper paper(m_pDb);
	long paperID;
	GetPropVal(FIELD_YDSTUMARK_PAPERID, &paperID);
	paper.SetID(paperID);
	std::list<CYDObjectRef*> lstStdAns;
	CListAutoClean<CYDObjectRef> clr(lstStdAns);
	HRESULT hr = paper.GetStdAnswer(&lstStdAns, NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	return CalMark(&lstStdAns, pMark);
}
HRESULT CYDStuMark::CalMark(std::list<CYDObjectRef*>* pLstStdAns, double* pMark)
{
	HRESULT hr = E_FAIL;
	std::map<int, CString> mapUserAnser;
	hr = TransferAnswer(&mapUserAnser);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrStd = pLstStdAns->begin();
	for (; itrStd != pLstStdAns->end(); ++itrStd)
	{
		long lNo;
		(*itrStd)->GetPropVal(FIELD_YDANSWER_NO, &lNo);
		CString strAnswer;
		(*itrStd)->GetPropVal(FIELD_YDANSWER_ANSWER, strAnswer);
		CComVariant varMark;
		(*itrStd)->GetPropVal(FIELD_YDANSWER_MARK, &varMark);
		std::map<int, CString>::const_iterator itrUser = mapUserAnser.find(lNo);
		if (itrUser != mapUserAnser.end())
		{
			if (itrUser->second.CompareNoCase(strAnswer) == 0)
			{
				*pMark += (double)_variant_t(varMark);;
			}
		}
	}

	return S_OK;
}

HRESULT CYDStuMark::CalMarkByString(double* pMark)
{
	CYDPaper paper(m_pDb);
	long paperID;
	GetPropVal(FIELD_YDSTUMARK_PAPERID, &paperID);
	paper.SetID(paperID);
	std::list<CYDObjectRef*> lstStdAns;
	CListAutoClean<CYDObjectRef> clr(lstStdAns);
	HRESULT hr = paper.GetStdAnswer(&lstStdAns, NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	return CalMarkByString(&lstStdAns, pMark);
}

HRESULT CYDStuMark::CalMarkByString(std::list<CYDObjectRef*>* pLstStdAns, double* pMark)
{
	HRESULT hr = E_FAIL;
	std::map<int, CString> mapUserAnser;
	hr = TransferAnswerByString(&mapUserAnser);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrStd = pLstStdAns->begin();
	for (; itrStd != pLstStdAns->end(); ++itrStd)
	{
		long lNo;
		(*itrStd)->GetPropVal(FIELD_YDANSWER_NO, &lNo);
		CString strAnswer;
		(*itrStd)->GetPropVal(FIELD_YDANSWER_ANSWER, strAnswer);
		CComVariant varMark;
		(*itrStd)->GetPropVal(FIELD_YDANSWER_MARK, &varMark);
		std::map<int, CString>::const_iterator itrUser = mapUserAnser.find(lNo);
		if (itrUser != mapUserAnser.end())
		{
			if (itrUser->second.CompareNoCase(strAnswer) == 0)
			{
				*pMark += (double)_variant_t(varMark);;
			}
		}
	}

	return S_OK;
}

HRESULT CYDStuMark::GetUnitsByStu(int _iPage,
									   std::list<CPropQueryContidition*> *_lstCondition,  
									   std::list<CYDObjectRef*>* pListObjs)
{
 	HRESULT hr = E_FAIL;
 	CString strSQL;
 	hr = CreateSQLFromQueryConditionByStu(_lstCondition, strSQL);
 	if (FAILED(hr))
 	{
 		return hr;
 	}
 	ASSERT(m_pDb != NULL);
 	DATABASE_TYPE dbType = m_pDb->GetDBType();
 	if(dbType == ACCESS ||
 		dbType == SQLSERVER)
 	{
 		int iStart = -1;
 		int iEnd = -1;
 		if(_iPage != -1)
 		{
 			iStart = _iPage * QUESTION_PAGE_COUNT + 1;
 			iEnd = iStart + QUESTION_PAGE_COUNT -1;
 		}
 		hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
 		if (FAILED(hr))
 		{
 			return hr;
 		}
 		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
 			itr != _lstCondition->end();++itr)
 		{
			if ((*itr)->m_uFieldType != VT_DATE)
			{
				hr = (*itr)->CreateParamter(m_pDb);
				if (FAILED(hr))
				{
					return hr;
				}
			}
 		}
 		hr = m_pDb->ExecuteSQL();
 		if(FAILED(hr))
 		{
 			return hr;
 		}
 
 		hr = AccessExeCreateObject(pListObjs,iStart,iEnd);
 		if (FAILED(hr))
 		{
 			return hr;
 		}
 	}
 	else if(dbType == ORACLE)
 	{
 		if(_iPage != -1)
 		{
 			int iRowStart = _iPage * QUESTION_PAGE_COUNT;
 			CString strRowStart;
 			strRowStart.Format(_T("%d"),iRowStart+1);
 			int iRowEnd = iRowStart + QUESTION_PAGE_COUNT;
 			CString strRowEnd;
 			strRowEnd.Format(_T("%d"),iRowEnd);
 			CString strBeginPageSQL = _T(" SELECT * FROM ( SELECT Z.*, ROWNUM RN FROM ( SELECT * FROM ( ");
 			CString strEndPageSQL = _T(" ) WHERE ROWNUM <= ");
 			strEndPageSQL += strRowEnd;
 			strEndPageSQL += _T(" ) Z ");
 			strEndPageSQL += _T(" ) WHERE RN >= ");
 			strEndPageSQL += strRowStart;
 			strSQL = strBeginPageSQL + strSQL;
 			strSQL = strSQL + strEndPageSQL;
 		}
 
 		hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
 		if (FAILED(hr))
 		{
 			return hr;
 		}
		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
			itr != _lstCondition->end();++itr)
		{
			hr = (*itr)->CreateParamter(m_pDb);
			if (FAILED(hr))
			{
				return hr;
			}
		}
 
 
 		hr = m_pDb->ExecuteSQL();
 		if(FAILED(hr))
 		{
 			return hr;
 		}
 
 		hr = OracleExeCreateObject(pListObjs);
 		if (FAILED(hr))
 		{
 			return hr;
 		}
 	}

	return S_OK;
}

HRESULT CYDStuMark::GetCountByCls(std::list<OBJID> *_lstClass, std::list<CPropQueryContidition*> *_lstCondition, long* pCount)
{
	HRESULT hr = E_FAIL;
	CString strSQLUnit = _T("SELECT objid FROM ");
	strSQLUnit += m_strDBName;
	strSQLUnit += L" ";
	BOOL bFirst = TRUE;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
// 		CString strItrSQL;
// 		hr = (*itr)->CreateContidition(strItrSQL);
// 		if (FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		if(strItrSQL.IsEmpty())
// 		{
// 			continue;
// 		}
// 		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) == 0)
// 		{
// 			strSQLUnit += strItrSQL;
// 		}
		CString strItrSQL;

		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			strItrSQL += L" cdate(";
			strItrSQL += FIELD_YDSTUMARK_EXAMDATE;
			strItrSQL += L") ";
			if ((*itr)->m_uOpType == Q_GE)
			{
				strItrSQL += L">= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LE)
			{
				strItrSQL += L"<= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_GREATOR)
			{
				strItrSQL += L"> cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LOWER)
			{
				strItrSQL += L"< cdate('";
			}
			else
			{
				continue;
			}
			strItrSQL += (*itr)->m_strConVal;
			strItrSQL += L"') ";
		}
		else
		{
			hr = (*itr)->CreateContidition(strItrSQL);
			if (FAILED(hr))
			{
				return hr;
			}
			if(strItrSQL.IsEmpty())
			{
				continue;
			}

		}
		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_SUBJECTID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			if(bFirst)
			{
				strSQLUnit += _T(" WHERE ");
				bFirst = FALSE;
			}
			else
			{
				strSQLUnit += _T(" AND ");
			}

			strSQLUnit += strItrSQL;
		}
	}

	CString strSQLStu = FIELD_YDSTUMARK_STUID;
	strSQLStu += L" in ( select id_b from  ";
	strSQLStu += DB_YDLINK_CLASS_STUDENT;
	strSQLStu += L" where id_a in (";
	std::list<OBJID>::const_iterator itr = _lstClass->begin();
	CString strID;
	strID.Format(L"%d", (*itr));
	++itr;
	strSQLStu += strID;
	for (; itr != _lstClass->end(); ++itr)
	{
		strSQLStu += L", ";
		strID.Format(L"%d", *itr);
		strSQLStu += strID;
	}
	strSQLStu += L") ";
	CString strSQL = _T("Select Count(*) As IDCOUNT From ( ") + strSQLUnit + L" and " + strSQLStu;
	strSQL += _T(" )) ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end();++itr)
	{
		hr = (*itr)->CreateParamter(m_pDb);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCount;
	hr = m_pDb->GetField(_variant_t(_T("IDCOUNT")),valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	*pCount = CDataHandler::VariantToLong(valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDStuMark::CreateSQLFromQueryCondition(std::list<CPropQueryContidition*> *_lstCondition,
												  CString &_strSQL)
{
	HRESULT hr = E_FAIL;
	_strSQL = _T("SELECT objid FROM ");
	_strSQL += m_strDBName;
	BOOL bFirst = TRUE;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
		CString strItrSQL;

		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			strItrSQL += L" cdate(";
			strItrSQL += FIELD_YDSTUMARK_EXAMDATE;
			strItrSQL += L") ";
			if ((*itr)->m_uOpType == Q_GE)
			{
				strItrSQL += L">= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LE)
			{
				strItrSQL += L"<= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_GREATOR)
			{
				strItrSQL += L"> cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LOWER)
			{
				strItrSQL += L"< cdate('";
			}
			else
			{
				continue;
			}
			strItrSQL += (*itr)->m_strConVal;
			strItrSQL += L"') ";
		}
		else
		{
			hr = (*itr)->CreateContidition(strItrSQL);
			if (FAILED(hr))
			{
				return hr;
			}
			if(strItrSQL.IsEmpty())
			{
				continue;
			}

		}
		
		if(bFirst)
		{
			_strSQL += _T(" WHERE ");
			bFirst = FALSE;
		}
		else
		{
			_strSQL += _T(" AND ");
		}

		_strSQL += strItrSQL;
		
	}


	return S_OK;
}

HRESULT CYDStuMark::CreateSQLFromQueryConditionByCls(std::list<OBJID> *_lstClass, std::list<CPropQueryContidition*> *_lstCondition,
										 CString &_strSQL)
{
	HRESULT hr = E_FAIL;
	CString strSQLUnit = _T("SELECT objid FROM ");
	strSQLUnit += m_strDBName;
	strSQLUnit += L" ";
	BOOL bFirst = TRUE;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
// 		CString strItrSQL;
// 		hr = (*itr)->CreateContidition(strItrSQL);
// 		if (FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		if(strItrSQL.IsEmpty())
// 		{
// 			continue;
// 		}
// 		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) == 0)
// 		{
// 			strSQLUnit += strItrSQL;
// 		}
		CString strItrSQL;
	
		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			strItrSQL += L" cdate(";
			strItrSQL += FIELD_YDSTUMARK_EXAMDATE;
			strItrSQL += L") ";
			if ((*itr)->m_uOpType == Q_GE)
			{
				strItrSQL += L">= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LE)
			{
				strItrSQL += L"<= cdate('";
			}
			else if ((*itr)->m_uOpType == Q_GREATOR)
			{
				strItrSQL += L"> cdate('";
			}
			else if ((*itr)->m_uOpType == Q_LOWER)
			{
				strItrSQL += L"< cdate('";
			}
			else
			{
				continue;
			}
			strItrSQL += (*itr)->m_strConVal;
			strItrSQL += L"') ";
		}
		else
		{
			hr = (*itr)->CreateContidition(strItrSQL);
			if (FAILED(hr))
			{
				return hr;
			}
			if(strItrSQL.IsEmpty())
			{
				continue;
			}

		}
		if ((*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_SUBJECTID) == 0 ||
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_EXAMDATE) == 0)
		{
			if(bFirst)
			{
				strSQLUnit += _T(" WHERE ");
				bFirst = FALSE;
			}
			else
			{
				strSQLUnit += _T(" AND ");
			}

			strSQLUnit += strItrSQL;
		}
	}

	CString strSQLStu = FIELD_YDSTUMARK_STUID;
	strSQLStu += L" in ( select id_b from  ";
	strSQLStu += DB_YDLINK_CLASS_STUDENT;
	strSQLStu += L" where id_a in (";
	std::list<OBJID>::const_iterator itr = _lstClass->begin();
	CString strID;
	strID.Format(L"%d", (*itr));
	++itr;
	strSQLStu += strID;
	for (; itr != _lstClass->end(); ++itr)
	{
		strSQLStu += L", ";
		strID.Format(L"%d", *itr);
		strSQLStu += strID;
	}
	strSQLStu += L")) ";
	_strSQL = strSQLUnit + L" and " + strSQLStu + _T(" ORDER BY OBJID  ");
	
	return S_OK;
}

HRESULT CYDStuMark::GetUnitsByCls(int _iPage,
									   std::list<OBJID> *_lstClass, 
									   std::list<CPropQueryContidition*> *_lstCondition, 
									   std::list<CYDObjectRef*>* pListObjs)
{

	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryConditionByCls(_lstClass, _lstCondition, strSQL);
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(m_pDb != NULL);
	DATABASE_TYPE dbType = m_pDb->GetDBType();
	if(dbType == ACCESS ||
		dbType == SQLSERVER)
	{
 		int iStart = -1;
 		int iEnd = -1;
 		if(_iPage != -1)
 		{
 			iStart = _iPage * QUESTION_PAGE_COUNT + 1;
 			iEnd = iStart + QUESTION_PAGE_COUNT -1;
 		}
 		hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
 		if (FAILED(hr))
 		{
 			return hr;
 		}
 	
 		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
 			itr != _lstCondition->end();++itr)
 		{
 			hr = (*itr)->CreateParamter(m_pDb);
 			if (FAILED(hr))
 			{
 				return hr;
 			}
 		}
 		hr = m_pDb->ExecuteSQL();
 		if(FAILED(hr))
 		{
 			return hr;
 		}
 
 		hr = AccessExeCreateObject(pListObjs,iStart,iEnd);
 		if (FAILED(hr))
 		{
 			return hr;
 		}
	}
	else if(dbType == ORACLE)
	{
		if(_iPage != -1)
		{
			int iRowStart = _iPage * QUESTION_PAGE_COUNT;
			CString strRowStart;
			strRowStart.Format(_T("%d"),iRowStart+1);
			int iRowEnd = iRowStart + QUESTION_PAGE_COUNT;
			CString strRowEnd;
			strRowEnd.Format(_T("%d"),iRowEnd);
			CString strBeginPageSQL = _T(" SELECT * FROM ( SELECT Z.*, ROWNUM RN FROM ( SELECT * FROM ( ");
			CString strEndPageSQL = _T(" ) WHERE ROWNUM <= ");
			strEndPageSQL += strRowEnd;
			strEndPageSQL += _T(" ) Z ");
			strEndPageSQL += _T(" ) WHERE RN >= ");
			strEndPageSQL += strRowStart;
			strSQL = strBeginPageSQL + strSQL;
			strSQL = strSQL + strEndPageSQL;
		}

		hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
		if (FAILED(hr))
		{
			return hr;
		}
		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
			itr != _lstCondition->end();++itr)
		{
			hr = (*itr)->CreateParamter(m_pDb);
			if (FAILED(hr))
			{
				return hr;
			}
		}


		hr = m_pDb->ExecuteSQL();
		if(FAILED(hr))
		{
			return hr;
		}

		hr = OracleExeCreateObject(pListObjs);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CYDStuMark::TransferAnswer(std::map<int, CString>* pMapAnswer)
{
	HRESULT hr = E_FAIL;

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDPaper paper(pDB);
	long paperID;
	GetPropVal(FIELD_YDSTUMARK_PAPERID, &paperID);
	paper.SetID(paperID);
	long begin1;
	paper.GetPropVal(FIELD_YDPAPER_BEGIN1, &begin1);
	long begin2;
	paper.GetPropVal(FIELD_YDPAPER_BEGIN2, &begin2);
	long begin3;

	paper.GetPropVal(FIELD_YDPAPER_BEGIN3, &begin3);
	long end1;
	paper.GetPropVal(FIELD_YDPAPER_END1, &end1);
	long end2;
	paper.GetPropVal(FIELD_YDPAPER_END2, &end2);
	long end3;
	paper.GetPropVal(FIELD_YDPAPER_END3, &end3);
	long num1;
	paper.GetPropVal(FIELD_YDPAPER_ANSWERCHOICE1, &num1);
	long num2;
	paper.GetPropVal(FIELD_YDPAPER_ANSWERCHOICE2, &num2);
	long num3;
	paper.GetPropVal(FIELD_YDPAPER_ANSWERCHOICE3, &num3);
	for (int i=begin1; i<=end1; ++i)
	{
		pMapAnswer->insert(std::map<int, CString>::value_type(i, L""));
	}
	for (int i=begin2; i<=end2; ++i)
	{
		pMapAnswer->insert(std::map<int, CString>::value_type(i, L""));
	}
	for (int i=begin3; i<=end3; ++i)
	{
		pMapAnswer->insert(std::map<int, CString>::value_type(i, L""));
	}

	CString strAnswer;
	hr = GetPropVal(FIELD_YDSTUMARK_ANSWER, strAnswer);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strNO, strOneAnswer;
	BOOL bFinished = FALSE;
	
	for (int i=0; i<strAnswer.GetLength(); ++i)
	{
		TCHAR c = strAnswer.GetAt(i);
		if (isdigit(c))
		{
			if (bFinished)
			{
				int nNo = _ttoi(strNO);
				std::map<int, CString>::iterator itrFind = pMapAnswer->find(nNo);
				if (itrFind != pMapAnswer->end())
				{
					itrFind->second = strOneAnswer;
				}
				strNO = L"";
				strOneAnswer = L"";
				bFinished = FALSE;
			}
			strNO += c;
		}
		else if (isalpha(c))
		{
			bFinished = TRUE;
			strOneAnswer += c;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	if (!strNO.IsEmpty() && !strOneAnswer.IsEmpty())
	{
		int nNo = _ttoi(strNO);
		std::map<int, CString>::iterator itrFind = pMapAnswer->find(nNo);
		if (itrFind != pMapAnswer->end())
		{
			itrFind->second = strOneAnswer;
		}
	}

	return S_OK;
}

HRESULT CYDStuMark::TransferAnswerByString(std::map<int, CString>* pMapAnswer)
{
	HRESULT hr = E_FAIL;
	CString strAnswer;
	hr = GetPropVal(FIELD_YDSTUMARK_ANSWER, strAnswer);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strNO, strOneAnswer;
	BOOL bFinished = FALSE;

	for (int i=0; i<strAnswer.GetLength(); ++i)
	{
		TCHAR c = strAnswer.GetAt(i);
		if (isdigit(c))
		{
			if (bFinished)
			{
				int nNo = _ttoi(strNO);
				pMapAnswer->insert(std::map<int, CString>::value_type(nNo, strOneAnswer));
				strNO = L"";
				strOneAnswer = L"";
				bFinished = FALSE;
			}
			strNO += c;
		}
		else if (isalpha(c))
		{
			bFinished = TRUE;
			strOneAnswer += c;
		}
		else
		{
			ASSERT(FALSE);
			strNO = L"";
			strOneAnswer = L"";
			bFinished = FALSE;
		}
	}
	if (!strNO.IsEmpty() && !strOneAnswer.IsEmpty())
	{
		int nNo = _ttoi(strNO);
		pMapAnswer->insert(std::map<int, CString>::value_type(nNo, strOneAnswer));
	}
	return S_OK;
}

HRESULT CYDStuMark::GetObjectTotalNumber(std::list<CPropQueryContidition*> *_lstCondition,
										   long* _lCount)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryCondition(_lstCondition,strSQL);
	if (FAILED(hr))
	{
		return hr;
	}
	strSQL = _T("Select Count(*) As IDCOUNT From ( ") + strSQL;
	strSQL += _T(" ) ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end();++itr)
	{
		if ((*itr)->m_uFieldType != VT_DATE &&
			(*itr)->m_strFiledName.CompareNoCase(FIELD_YDSTUMARK_PAPERID) != 0)
		{
			hr = (*itr)->CreateParamter(m_pDb);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCount;
	hr = m_pDb->GetField(_variant_t(_T("IDCOUNT")),valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	*_lCount = CDataHandler::VariantToLong(valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

