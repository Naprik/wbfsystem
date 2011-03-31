#include "StdAfx.h"
#include "YdStudentRef.h"
#include "../\ObjRef\PropQueryContidition.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "YDClassRef.h"
#include "YDExamAddress.h"
#include "YDDepartmentRef.h"
#include "YDEAddrUnit.h"
#include "../Include\YDVaultError.h"



CYdStudentRef::CYdStudentRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDSTUINFO;
	AddPropDef(FIELD_YDSTU_ID,VT_BSTR);//学号
	AddPropDef(FIELD_YDSTU_NAME,VT_BSTR);	//学生姓名
	AddPropDef(FIELD_YDSTU_GENDER,VT_I4,_T("1"));	//学生性别
	AddPropDef(FIELD_YDSTU_AGE,VT_I4);	//学生年龄
	AddPropDef(FIELD_YDSTU_TEL,VT_BSTR);	//学生电话
	AddPropDef(FIELD_YDSTU_MAIL,VT_BSTR);	//学生Emai
	AddPropDef(FIELD_YDSTU_PHOTO,VT_BLOB_OBJECT);	//学生图片	
	AddPropDef(FIELD_YDSTU_EVID,VT_BSTR);	//设备ID	
}

CYdStudentRef::~CYdStudentRef(void)
{
}

HRESULT CYdStudentRef::GetClass(std::list<CYDObjectRef*>* pListClasses, std::list<CYDLinkRef*>* pLinks /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_A FROM ");
	strSQL += DB_YDLINK_CLASS_STUDENT;
	strSQL += _T(" WHERE ID_B = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_B", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(m_uObjID),&vtParam);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	while(!m_pDb->IsEOF())
	{
		_variant_t val;
		hr = m_pDb->GetField(_variant_t("ID_A"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idClass = CDataHandler::VariantToLong(val);
		CYDClassRef* pClass = new CYDClassRef(m_pDb);
		pListClasses->push_back(pClass);
		hr = pClass->SetID(idClass);
		if(FAILED(hr))
		{
			return hr;
		}
		if(pLinks != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_YDLINK_CLASS_STUDENT);
			hr = pLinkRef->PutObjRef(pClass, this);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			pLinks->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

//根据条件得到对应的学生信息
HRESULT CYdStudentRef::GetStudents(CDatabaseEx* pDB,
								  std::list<CYdStudentRef*>* _lstref,
								  std::list<CPropQueryContidition*>* _lstQueryCondition /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT OBJID FROM ");
	strSQL += DB_YDSTUINFO;
	if(_lstQueryCondition != NULL)
	{
		BOOL bFirst = TRUE;
		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstQueryCondition->begin();
			itr != _lstQueryCondition->end();++itr)
		{
			CString strConditionSQL;
			hr = (*itr)->CreateContidition(strConditionSQL);
			if(FAILED(hr))
			{
				return hr;
			}
			if (bFirst)
			{
				strSQL += _T(" WHERE ");
				bFirst = FALSE;
			}
			else
			{
				strSQL += _T(" AND ");
			}
			strSQL += strConditionSQL;
		}
	}
	hr = pDB->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstQueryCondition->begin();
		itr != _lstQueryCondition->end();++itr)
	{
		if((*itr)->m_strConVal.IsEmpty())
		{
			continue;
		}
		CString strFieldName = (*itr)->m_strFiledName;
		CComVariant valProp((*itr)->m_strConVal);
		ADODB::DataTypeEnum DataType ;
		int size = 0;
		CDataHandler::VarTypeToDBType(&valProp,DataType,size);
		hr = pDB->AddParameter(_bstr_t(strFieldName), DataType, adParamInput, 
			size, &valProp);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	hr = pDB->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	while(!pDB->IsEOF())
	{
		_variant_t valOBJID;
		hr = pDB->GetField(_variant_t(_T("OBJID")),valOBJID);
		if(FAILED(hr))
		{
			return hr;
		}
		OBJID uID = CDataHandler::VariantToLong(valOBJID);
		CYdStudentRef* pRef = new CYdStudentRef(pDB);
		hr = pRef->SetID(uID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pDB->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
		
	}
	return S_OK;
}

HRESULT CYdStudentRef::GetGender(CString* pStrGender)
{
	CComVariant var;
	GetPropVal(FIELD_YDSTU_GENDER, &var);
	long lGender = CDataHandler::VariantToLong(var);
	if (lGender == 0)
	{
		*pStrGender = L"男";
	}
	else if (lGender == 1)
	{
		*pStrGender = L"女";
	}
	else
	{
		*pStrGender = L"";
	}

	return S_OK;
}

HRESULT CYdStudentRef::SetGender(const CString& strGender)
{
	long lGender = 0;
	if (strGender.CompareNoCase(L"男") == 0)
	{
		lGender = 0;
	}
	else if (strGender.CompareNoCase(L"女") == 0)
	{
		lGender = 1;
	}
	CComVariant varGender((long)lGender);
	SetPropVal(FIELD_YDSTU_GENDER, &varGender);
	

	return S_OK;
}

HRESULT CYdStudentRef::GetStudentByConditionFromDeptAndCls(int _iPage, 
															 OBJID deptID, 
															 OBJID classID,
															 std::list<CPropQueryContidition*> *_lstCondition,
															 std::list<CYDObjectRef*>* _lstObjects)
{
	if (deptID == ID_EMPTY && 
		classID == ID_EMPTY)
	{
		return GetObjectByCondition(_iPage, _lstCondition, _lstObjects);
	}
	if (classID == ID_EMPTY)//
	{
		return GetStudentByConditionFromDept(_iPage, deptID, _lstCondition, _lstObjects);
	}
	else
	{
		return GetStudentByConditionFromCls(_iPage, classID, _lstCondition, _lstObjects);
	}
	
	return S_OK;
}

HRESULT CYdStudentRef::GetStudentByConditionFromDept(int _iPage, 
													  OBJID deptID, 
													  std::list<CPropQueryContidition*> *_lstCondition,
													  std::list<CYDObjectRef*>* _lstObjects)
{
	CYDDepartmentRef deptRef(m_pDb);
	deptRef.SetID(deptID);
	std::list<CYDObjectRef*> lstCls;
	CListAutoClean<CYDObjectRef> clr(lstCls);
	HRESULT hr = deptRef.GetAllClasses(&lstCls, NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	for (std::list<CYDObjectRef*>::const_iterator itr = lstCls.begin();
		 itr != lstCls.end(); ++itr)
	{
		OBJID clsID = ID_EMPTY;
		(*itr)->GetID(&clsID);
		hr = GetStudentByConditionFromCls(_iPage, clsID, _lstCondition, _lstObjects);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
HRESULT CYdStudentRef::GetStudentByConditionFromCls(int _iPage, 
													 OBJID clsID, 
													 std::list<CPropQueryContidition*> *_lstCondition,
													 std::list<CYDObjectRef*>* _lstObjects)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryConditionByCls(_lstCondition, strSQL);
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
		_variant_t vtParam((long)clsID);
		hr = m_pDb->AddParameter(L"id_a", 
			adUnsignedInt, 
			adParamInput, 
			sizeof(clsID),&vtParam);
		if(FAILED(hr))
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

		hr = AccessExeCreateObject(_lstObjects,iStart,iEnd);
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
		_variant_t vtParam((long)clsID);
		hr = m_pDb->AddParameter(L"id_a", 
			adUnsignedInt, 
			adParamInput, 
			sizeof(clsID),&vtParam);
		if(FAILED(hr))
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

		hr = OracleExeCreateObject(_lstObjects);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CYdStudentRef::CreateSQLFromQueryConditionByCls(std::list<CPropQueryContidition*> *_lstCondition,
											 CString &_strSQL)
{
	HRESULT hr = E_FAIL;
	_strSQL = _T("SELECT objid FROM ");
	_strSQL += m_strDBName;
	_strSQL += _T(" where objid in( select id_b from clsstu where id_a = ?) ");
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
		CString strItrSQL;
		hr = (*itr)->CreateContidition(strItrSQL);
		if (FAILED(hr))
		{
			return hr;
		}
		if(strItrSQL.IsEmpty())
		{
			continue;
		}
		
		_strSQL += _T(" AND ");
		

		_strSQL += strItrSQL;
	}
	

	_strSQL += _T(" ORDER BY OBJID  ");

	return S_OK;
}

CYDObjectRef* CYdStudentRef::CreateObject()
{
	return new CYdStudentRef(m_pDb);
}

HRESULT CYdStudentRef::GetStudentTotalNumber(OBJID deptID, 
							  OBJID classID,
							  std::list<CPropQueryContidition*> *_lstCondition,
							  long* _lCount)
{
	if (deptID == ID_EMPTY && 
		classID == ID_EMPTY)
	{
		return GetObjectTotalNumber(_lstCondition, _lCount);
	}
	if (classID == ID_EMPTY)//
	{
		return GetStudentTotalNumberOfDept(deptID, _lstCondition, _lCount);
	}
	else
	{
		return GetStudentTotalNumberOfClsr(classID, _lstCondition, _lCount);
	}

	return S_OK;
}

HRESULT CYdStudentRef::GetStudentTotalNumberOfClsr(OBJID classID,
							  std::list<CPropQueryContidition*> *_lstCondition,
							  long* _lCount)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryConditionByCls(_lstCondition,strSQL);
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
	_variant_t vtParam((long)classID);
	hr = m_pDb->AddParameter(L"id_a", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(classID),&vtParam);
	if(FAILED(hr))
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
	*_lCount = CDataHandler::VariantToLong(valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYdStudentRef::GetStudentTotalNumberOfDept(OBJID deptID,
						   std::list<CPropQueryContidition*> *_lstCondition,
						   long* _lCount)
{
	CYDDepartmentRef deptRef(m_pDb);
	deptRef.SetID(deptID);
	std::list<CYDObjectRef*> lstCls;
	CListAutoClean<CYDObjectRef> clr(lstCls);
	HRESULT hr = deptRef.GetAllClasses(&lstCls, NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	for (std::list<CYDObjectRef*>::const_iterator itr = lstCls.begin();
		itr != lstCls.end(); ++itr)
	{
		OBJID clsID = ID_EMPTY;
		(*itr)->GetID(&clsID);
		long count = 0;
		hr = GetStudentTotalNumberOfClsr(clsID, _lstCondition, &count);
		if (FAILED(hr))
		{
			return hr;
		}
		*_lCount += count;
	}
	return S_OK;
}

HRESULT CYdStudentRef::Remove()
{
	HRESULT hr = E_FAIL;
	{
		//将学生与班级的关系删除
		std::list<CYDObjectRef*> lstClass;
		std::list<CYDLinkRef*> lstClassLink;
		hr = GetClass(&lstClass,&lstClassLink);
		if(FAILED(hr))
		{
			return hr;
		}
		CListAutoClean<CYDObjectRef> clr1(lstClass);
		CListAutoClean<CYDLinkRef> clr2(lstClassLink);
		ASSERT(lstClass.size() == lstClassLink.size());
		for(std::list<CYDLinkRef*>::const_iterator itr = lstClassLink.begin();
			itr != lstClassLink.end();++itr)
		{
			hr = (*itr)->Remove();
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}

	{
		//将该学生属于的考试单元从其中删除
		std::list<CYDObjectRef*> lstUnit;
		hr = GetInAddrUnit(lstUnit);
		if(FAILED(hr))
		{
			return hr;
		}
		CListAutoClean<CYDObjectRef> clr2(lstUnit);
		for(std::list<CYDObjectRef*>::const_iterator itr = lstUnit.begin();
			itr != lstUnit.end();++itr)
		{
			hr = (*itr)->Remove();
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}

	hr = CYDObjectRef::Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYdStudentRef::GetInAddrUnit(std::list<CYDObjectRef*>& _lstUnit)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT OBJID FROM ");
	strSQL += DB_YDEXAMADDRUNIT ;
	strSQL += _T(" WHERE ");
	strSQL += FIELD_YDEXAMADDRUNIT_STUID;
	strSQL += _T(" = ? ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valProp((long)m_uObjID);
	ADODB::DataTypeEnum DataType ;
	int size = 0;
	CDataHandler::VarTypeToDBType(&valProp,DataType,size);
	hr = m_pDb->AddParameter(_bstr_t(FIELD_YDEXAMADDRUNIT_STUID), DataType, adParamInput, 
				size, &valProp);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	while(!m_pDb->IsEOF())
	{
		_variant_t valOBJID;
		hr = m_pDb->GetField(_variant_t(_T("OBJID")),valOBJID);
		if(FAILED(hr))
		{
			return hr;
		}
		OBJID uID = CDataHandler::VariantToLong(valOBJID);
		CYDObjectRef* pRef = new CYDEAddrUnit(m_pDb);
		ASSERT(pRef);
		hr = pRef->SetID(uID);
		if(FAILED(hr))
		{
			return hr;
		}
		_lstUnit.push_back(pRef);
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}

	}
	return S_OK;
}


HRESULT CYdStudentRef::Restore(BOOL bReload /*= FALSE*/)
{
	HRESULT hr = E_FAIL;
	if(bReload)
	{
		m_mapProp.clear();
	}
	if(m_mapProp.size() > 0)
	{
		return S_OK;
	}
	_RecordsetPtr pRecordset = NULL;
	hr = GetCurRecord(pRecordset);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pRecordset);
	for(std::list<CYDPropDef*>::const_iterator itr = m_lstPropDef.begin();
		itr != m_lstPropDef.end(); ++itr)
	{
		_variant_t val;
		CString strPropName = (*itr)->m_strPropName;
		if((*itr)->m_uDataType == VT_BLOB_OBJECT)
		{
			long lDataSize = pRecordset->GetFields()->GetItem(_variant_t(strPropName))->ActualSize;
			if(lDataSize > 0)
			{				
				val = pRecordset->GetFields()->GetItem(_variant_t(strPropName))->GetChunk(lDataSize);
			}
		}
		else
		{
			val = pRecordset->GetCollect(_variant_t(strPropName));
		}
		m_mapProp.insert(MapProp::value_type(strPropName,val));
	}
	return S_OK;
}
HRESULT CYdStudentRef::Update()
{
	HRESULT hr = E_FAIL;
	_RecordsetPtr pRecordset = NULL;
	hr = GetCurRecord(pRecordset);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pRecordset);
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		CString strPropName = (*itrProp)->m_strPropName;
		_variant_t valProp;
		hr = GetPropVal(_bstr_t(strPropName),&valProp);
		if(FAILED(hr))
		{
			return hr;
		}
		if((*itrProp)->m_uDataType == VT_BLOB_OBJECT)
		{
			//对象Image图像，要特殊处理
			if(valProp.vt != VT_EMPTY)
			{
				pRecordset->GetFields()->GetItem(_variant_t(strPropName))->AppendChunk(valProp);
			}
		}
		else
		{
			pRecordset->PutCollect(CComVariant(strPropName),valProp);
		}

	}
	hr = pRecordset->Update();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYdStudentRef::OracleSave()
{
	HRESULT hr = E_FAIL;
	hr = OracleSaveID();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = AddNewRecord();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYdStudentRef::SQLServerSave()
{
	HRESULT hr = E_FAIL;
	hr = AddNewRecord();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = SQLServerSaveID();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYdStudentRef::AccessSave()
{
	HRESULT hr = E_FAIL;
	hr = AccessSaveID();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = AddNewRecord();
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CYdStudentRef::AddNewRecord()
{
	HRESULT hr = E_FAIL;
	CComVariant valID;
	valID.vt = VT_I4;
	valID.lVal = m_uObjID;
	CString strSQL = _T("Select * From ");
	strSQL += m_strDBName;
	_RecordsetPtr pRecordset = NULL;
	pRecordset.CreateInstance("ADODB.Recordset");
	CComVariant valConnection;
	valConnection.vt = VT_DISPATCH;
	_ConnectionPtr pConnection = m_pDb->GetConnection();
	hr = pConnection->QueryInterface(&valConnection.pdispVal);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pRecordset->Open(CComVariant(strSQL),valConnection,
		adOpenStatic,adLockOptimistic,adCmdText);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pRecordset->AddNew();
	if(FAILED(hr))
	{
		return hr;
	}
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		CString strPropName = (*itrProp)->m_strPropName;
		_variant_t valProp;
		hr = GetPropVal(_bstr_t(strPropName),&valProp);
		if(FAILED(hr))
		{
			return hr;
		}
		if((*itrProp)->m_uDataType == VT_BLOB_OBJECT)
		{
			//对象Image图像，要特殊处理
			if(valProp.vt != VT_EMPTY)
			{
				pRecordset->GetFields()->GetItem(_variant_t(strPropName))->AppendChunk(valProp);
			}
		}
		else
		{
			pRecordset->PutCollect(CComVariant(strPropName),valProp);
		}

	}
	pRecordset->PutCollect(L"ObjID",&valID);
	hr = pRecordset->Update();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}