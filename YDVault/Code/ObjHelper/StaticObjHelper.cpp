#include "StdAfx.h"
#include "StaticObjHelper.h"
#include "../ObjRef/PropQueryContidition.h"
#include "../DBBase/DatabaseEx.h"
#include "../base/DataHandler.h"
#include "../ObjRef/YDObjectRef.h"
#include "../YDExamObjRef/YdStudentRef.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../YDExamObjRef\YDClassRef.h"
#include "../YDExamObjRef\YDTeacherDevRef.h"
#include "../YDExamObjRef\YDSubjectRef.h"
#include "../ObjRef/YDPaper.h"
#include "../ObjRef/YDAnswer.h"
#include "../YDExamObjRef\YDResultRef.h"
#include "../YDExamObjRef\YDExamAddress.h"
#include "../YDExamObjRef/YDDevice.h"
#include "../YDExamObjRef\YDExamInfo.h"
#include "../\ObjRef\YDUserRef.h"
#include "../ObjRef\YDQuestionVault.h"
#include "../YDExamObjRef\YDStuMark.h"

CStaticObjHelper::CStaticObjHelper(void)
{
}

CStaticObjHelper::~CStaticObjHelper(void)
{
}

HRESULT CStaticObjHelper::GetObjRef(CString _strDBName,
									CDatabaseEx* pDB,
									std::list<CYDObjectRef*>* _lstref,
									std::list<CPropQueryContidition*>* _lstQueryCondition /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT OBJID FROM ");
	strSQL += _strDBName;
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
	if(_lstQueryCondition != NULL)
	{
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
		CYDObjectRef* pRef = NULL;
		hr = CreateObjRefByDBName(_strDBName,pDB,pRef);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(pRef);
		hr = pRef->SetID(uID);
		if(FAILED(hr))
		{
			return hr;
		}
		_lstref->push_back(pRef);
		hr = pDB->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}

	}
	return S_OK;
}

HRESULT CStaticObjHelper::CopyObj(CYDObjectRef* _pSourceObj,
								  CDatabaseEx* pDB,
								CYDObjectRef* &_pNewObj)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pSourceObj);
	CString strDBName;
	hr = _pSourceObj->GetDBName(strDBName);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = CreateObjRefByDBName(strDBName,pDB,_pNewObj);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(_pNewObj);
	UINT uID = 0;
	hr = _pSourceObj->GetID(&uID);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pNewObj->SetID(uID);
	if(FAILED(hr))
	{
		return hr;
	}
	//拷贝属性值
	UINT iCount = 0;
	hr = _pSourceObj->GetPropCount(&iCount);
	if(FAILED(hr))
	{
		return hr;
	}
	for(UINT i = 0; i < iCount;i++)
	{
		CComBSTR bstrName;
		CComVariant val;
		hr = _pSourceObj->GetProp(i,&bstrName,&val);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = _pNewObj->SetPropVal(bstrName,&val);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CStaticObjHelper::CreateObjRefByDBName(CString _strDBName,
											   CDatabaseEx* pDB,
									CYDObjectRef* &_pRef)
{
	HRESULT hr = E_FAIL;
	ASSERT(!_pRef);
	if(_strDBName.CompareNoCase(DB_ARTICLEQUESTION) == 0)
	{

	}
	else if(_strDBName.CompareNoCase(DB_CHOICEQUESTION) == 0)
	{

	}
	else if(_strDBName.CompareNoCase(DB_YDSTUINFO) ==0)
	{
		//学生信息
		_pRef = new CYdStudentRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDDEPARTMENT) == 0)
	{
		//部门信息
		_pRef = new CYDDepartmentRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDCLASS) == 0)
	{
		//班级信息
		_pRef = new CYDClassRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDTEACHERDEV) == 0)
	{
		//教师机地址信息
		_pRef = new CYDTeacherDevRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDSUBJECT) == 0)
	{
		//考试科目信息表
		_pRef = new CYDSubjectRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_PAPER) == 0)
	{
		//考卷信息表
		_pRef = new CYDPaper(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_ANSWER) == 0)
	{
		//标准答案信息表
		_pRef = new CYDAnswer(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDRESULT) == 0)
	{
		//学生答案信息表
		_pRef = new CYDResultRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDEXAMADDR) == 0)
	{
		//考场信息
		_pRef = new CYDExamAddress(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDDEVICE) == 0)
	{
		//设备
		_pRef = new CYDDevice(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDEXAM_INFO) == 0)
	{
		//监考信息
		_pRef = new CYDExamInfo(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDUSER) == 0)
	{
		//登陆用户
		_pRef = new CYDUserRef(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_QUESTIONVAULT) == 0)
	{
		//题库
		_pRef = new CYDQuestionVault(pDB);
	}
	else if(_strDBName.CompareNoCase(DB_YDSTUMARK) == 0)
	{
		//题库
		_pRef = new CYDStuMark(pDB);
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}


