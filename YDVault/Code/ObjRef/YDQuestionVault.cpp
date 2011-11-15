#include "StdAfx.h"
#include "YDQuestionVault.h"
#include "../DBBase/DatabaseEx.h"
#include "../base/DataHandler.h"
#include "YdKnowledge.h"
#include "YDLinkRef.h"
#include "../Base/AutoClean.h"
#include "YDQuestionType.h"
#include "YDChoiceQuestionRef.h"
#include "YDArticleQuestionRef.h"
#include "PropQueryContidition.h"

CYDQuestionVault::CYDQuestionVault(CDatabaseEx* pdb) : CYDObjectRef(pdb) 
{
	m_strDBName = DB_QUESTIONVAULT;
	CYDPropDef* pPropDef = new CYDPropDef(FIELD_YDVAULT_NAME,VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("CREATOR"),VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("CREATEDATE"),VT_DATE);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("DESCRIPTION"),VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	m_pQuestionRecord = NULL;
}

CYDQuestionVault::~CYDQuestionVault(void)
{
	m_pQuestionRecord = NULL;
}

HRESULT CYDQuestionVault::GetAllKnowledgePoint(std::list<CYdKnowledge*>* sub_knowledge,
						  std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_QVAULTKPOINT;
	strSQL += _T(" WHERE ID_A = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", 
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
		hr = m_pDb->GetField(_variant_t("ID_B"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idChild = CDataHandler::VariantToLong(val);
		CYdKnowledge* pChild = new CYdKnowledge(m_pDb);
		sub_knowledge->push_back(pChild);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			//失败清除
			CListAutoClean<CYdKnowledge> clean(*sub_knowledge);
			return hr;
		}
		if(_sub_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_QVAULTKPOINT);
			hr = pLinkRef->PutObjRef(this,pChild);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			_sub_link->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetAllQuestionType(std::list<CYDQuestionType*>* _sub_qtype,
							 std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_QVAULTQTYPE;
	strSQL += _T(" WHERE ID_A = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", 
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
		hr = m_pDb->GetField(_variant_t("ID_B"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idChild = CDataHandler::VariantToLong(val);
		CYDQuestionType* pChild = new CYDQuestionType(m_pDb);
		_sub_qtype->push_back(pChild);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			//失败清除
			CListAutoClean<CYDQuestionType> clean(*_sub_qtype);
			return hr;
		}
		if(_sub_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_QVAULTQTYPE);
			hr = pLinkRef->PutObjRef(this,pChild);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			_sub_link->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetAllQuestion(std::list<CYDQuestionRef*>* _sub_question,
					   std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B,ID_TYPE FROM ");
	strSQL += DB_VAULTQUESTION;
	strSQL += _T(" WHERE ID_A = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", 
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
	std::list<CYDID*> lstYDID;
	CListAutoClean<CYDID> clr(lstYDID);
	while(!m_pDb->IsEOF())
	{
		CYDID* pYDID = new CYDID;
		_variant_t val;
		hr = m_pDb->GetField(_variant_t("ID_B"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		pYDID->m_IDB = CDataHandler::VariantToLong(val);
		_variant_t valLinkID;
		hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
		if(FAILED(hr))
		{
			return hr;
		}
		pYDID->m_objID = CDataHandler::VariantToLong(valLinkID);
		_variant_t valTypeID;
		hr = m_pDb->GetField(_variant_t(_T("ID_TYPE")),valTypeID);
		if(FAILED(hr))
		{
			return hr;
		}
		pYDID->m_IDType = CDataHandler::VariantToLong(valTypeID);
		lstYDID.push_back(pYDID);
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	for(std::list<CYDID*>::const_iterator itr = lstYDID.begin();
		itr != lstYDID.end();++itr)
	{
		QUESTION_TYPE QuestionType = QUESTION_UNKNOWN;
		hr = GetQuestionTypeIDDB((*itr)->m_IDType,QuestionType);
		if(FAILED(hr))
		{
			return hr;
		}
		CYDQuestionRef* pQuestionRef = NULL;

		if(QuestionType == QUESTION_ARTICLE )
		{
			pQuestionRef = new CYDArticleQuestionRef(m_pDb);
		}
		else if(QuestionType == QUESTION_CHOICE	)
		{
			pQuestionRef = new CYDChoiceQuestionRef(m_pDb);
		}
		else 
		{
			ASSERT(FALSE);
		}
		ASSERT(pQuestionRef);
		hr = pQuestionRef->SetID((*itr)->m_IDB);
		if(FAILED(hr))
		{
			return hr;
		}
		_sub_question->push_back(pQuestionRef);
		if(_sub_link != NULL)
		{
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_VAULTQUESTION);
			hr = pLinkRef->PutObjRef(this,pQuestionRef);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID((*itr)->m_objID);
			if(FAILED(hr))
			{
				return hr;
			}
			_sub_link->push_back(pLinkRef);
		}
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetQuestionTypeIDDB(UINT _idQuestionType,
							QUESTION_TYPE& _qType)
{
	HRESULT hr = E_FAIL;
	CYDQuestionType QType(m_pDb);
	hr = QType.SetID(_idQuestionType);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = QType.Restore();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t val;
	hr = QType.GetPropVal(FIELD_QUESTIONTYPE_TYPE,&val);
	if(FAILED(hr))
	{
		return hr;
	}

	QTYPE uQType = (QTYPE)CDataHandler::VariantToLong(val);
	if( uQType ==  QTYPE_READINGCOMPREHENSION ||//阅读理解题
		uQType == QTYPE_LISTENINGCOMPREHENSION || //
		uQType == QTYPE_CLOZE ||
		uQType == QTYPE_LISTENINGVOCABULARY ||
		uQType == QTYPE_SKIMMING ||
		uQType == QTYPE_READINGVOCABULARY ||
		uQType == QTYPE_LISTENINGLONG ||
		uQType == QTYPE_LISTENINGSHORT ||
		uQType == QTYPE_ERRORCORRECTION
		)
	{
		_qType = QUESTION_ARTICLE;//ArticleQuestionRef
	}
	else  if(
		uQType == QTYPE_WRITING ||//写作题
		uQType == QTYPE_VOCABULARY  ||
		uQType == QTYPE_TRANSLATION //翻译
		)
	{
		_qType = QUESTION_CHOICE;//ChoiceQuestionRef
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetQuestionByTypeID(UINT _idQuestionType,
							std::list<CYDQuestionRef*>* _sub_question,
							std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;

	CYDQuestionType questionType(m_pDb);
	questionType.SetID(_idQuestionType);
	CComVariant varType;
	questionType.GetPropVal(FIELD_QUESTIONTYPE_TYPE, &varType);
	QTYPE qtype = (QTYPE)CDataHandler::VariantToLong(varType);

	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_VAULTQUESTION;
	strSQL += _T(" WHERE ID_A = ? and ID_TYPE = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(m_uObjID),&vtParam);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam2((long)_idQuestionType);
	hr = m_pDb->AddParameter(L"ID_TYPE", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(_idQuestionType),&vtParam2);
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
		_variant_t varObjID;
		hr = m_pDb->GetField(L"OBJID", varObjID);
		if (FAILED(hr))
		{
			return hr;
		}
		long linkID = CDataHandler::VariantToLong(varObjID);
		_variant_t val;
		hr = m_pDb->GetField(_T("ID_B"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idChild = CDataHandler::VariantToLong(val);
		CYDQuestionRef* pQuestionRef = NULL;

		if(
			qtype == QTYPE_READINGCOMPREHENSION ||//阅读理解题
			qtype == QTYPE_LISTENINGCOMPREHENSION || //
			qtype == QTYPE_CLOZE ||
			qtype == QTYPE_LISTENINGVOCABULARY ||
			qtype == QTYPE_SKIMMING ||
			qtype == QTYPE_READINGVOCABULARY ||
			qtype == QTYPE_LISTENINGLONG ||
			qtype == QTYPE_LISTENINGSHORT ||
			qtype == QTYPE_ERRORCORRECTION
			)
		{
			pQuestionRef = new CYDArticleQuestionRef(m_pDb);
		}
		else if(
			qtype == QTYPE_WRITING ||//写作题
			qtype == QTYPE_VOCABULARY ||
			qtype == QTYPE_TRANSLATION //翻译
			)
		{
			//选择题
			pQuestionRef = new CYDChoiceQuestionRef(m_pDb);
		}
		else 
		{
			ASSERT(FALSE);
		}
		ASSERT(pQuestionRef);
		hr = pQuestionRef->SetID(idChild);
		if(FAILED(hr))
		{
			return hr;
		}
		_sub_question->push_back(pQuestionRef);
		if(_sub_link != NULL)
		{
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_VAULTQUESTION);
			hr = pLinkRef->PutObjRef(this,pQuestionRef);
			if(FAILED(hr))
			{
				return hr;
			}
			
			hr = pLinkRef->SetID(linkID);
			if(FAILED(hr))
			{
				return hr;
			}
			_sub_link->push_back(pLinkRef);
		}

		hr = m_pDb->MoveNext();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetQuestionByTypeIDCondition(UINT _idQuestionType,
													   int _iPage,
									 std::list<CPropQueryContidition*> *_lstCondition,
									 std::list<CYDQuestionRef*>* _sub_question,
									 std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	QUESTION_TYPE QuestionTYpe = QUESTION_UNKNOWN;
	hr = GetQuestionTypeIDDB(_idQuestionType,QuestionTYpe);
	if (FAILED(hr))
	{
		return hr;
	}
	if(QuestionTYpe == QUESTION_CHOICE)
	{
		//ChoiceQuestion
		hr = GetChoiceQuestionByTypeIDCondition(_idQuestionType,
												_iPage,
												_lstCondition,
												_sub_question,
												_sub_link);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else if(QuestionTYpe == QUESTION_ARTICLE)
	{
		//ArticleQuestin
		hr = GetArticleQuestionByTypeIDCondition(_idQuestionType,
												 _iPage,
												 _lstCondition,
												 _sub_question,
												 _sub_link);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetQuestionByTypeIDConditionCount(UINT _idQuestionType,
										  std::list<CPropQueryContidition*> *_lstCondition,
										  long* _lCount)
{
	HRESULT hr = E_FAIL;
	QUESTION_TYPE QuestionTYpe = QUESTION_UNKNOWN;
	hr = GetQuestionTypeIDDB(_idQuestionType,QuestionTYpe);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strDBName ;
	if(QuestionTYpe == QUESTION_CHOICE)
	{
		//ChoiceQuestion
		strDBName = DB_CHOICEQUESTION;
	}
	else if(QuestionTYpe == QUESTION_ARTICLE)
	{
		//ArticleQuestin
		strDBName = DB_ARTICLEQUESTION;
	}
	else
	{
		ASSERT(FALSE);
	}
	hr = GetQuestionCount(_idQuestionType,strDBName,_lstCondition,_lCount);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDQuestionVault::GetQuestionCount(
	UINT _idQuestionType,
	CString  _strDBName,
	std::list<CPropQueryContidition*> *_lstCondition,
	long* _lCount)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateConditionSQL(_strDBName,_lstCondition,strSQL);
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

	hr = CreateParamter(_idQuestionType,_lstCondition);
	if (FAILED(hr))
	{
		return hr;
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

HRESULT CYDQuestionVault::GetChoiceQuestionByTypeIDCondition(UINT _idQuestionType,
															 int _iPage,
										   std::list<CPropQueryContidition*> *_lstCondition,
										   std::list<CYDQuestionRef*>* _sub_question,
										   std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	if(_iPage == 0)
	{
		//说明是第一次
		hr = ExeConditionDB(_idQuestionType,DB_CHOICEQUESTION,_lstCondition);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	hr = ExeDBCreateQuestion(QUESTION_CHOICE,
		_sub_question,
		_sub_link);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYDQuestionVault::GetArticleQuestionByTypeIDCondition(UINT _idQuestionType,
															  int _iPage,
											std::list<CPropQueryContidition*> *_lstCondition,
											std::list<CYDQuestionRef*>* _sub_question,
											std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	if(_iPage == 0)
	{
		//说明是第一次
		hr = ExeConditionDB(_idQuestionType,DB_ARTICLEQUESTION,_lstCondition);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	hr = ExeDBCreateQuestion(QUESTION_ARTICLE,
							_sub_question,
							_sub_link);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}



HRESULT CYDQuestionVault::ExeConditionDB(UINT _idQuestionType,
										  CString _strDBName,
					   std::list<CPropQueryContidition*> *_lstCondition)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateConditionSQL(_strDBName,_lstCondition,strSQL);
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(m_pDb != NULL);
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateParamter(_idQuestionType,_lstCondition);
	if (FAILED(hr))
	{
		return hr;
	}
	
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	m_pQuestionRecord = m_pDb->GetRecordset();
	return S_OK;
}

HRESULT CYDQuestionVault::CreateParamter(UINT _idQuestionType,
										 std::list<CPropQueryContidition*> *_lstCondition)
{
	HRESULT hr = E_FAIL;
	if(_lstCondition != NULL)
	{
		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
			itr != _lstCondition->end();++itr)
		{
			hr = (*itr)->CreateParamter(m_pDb);
			if (FAILED(hr))
			{
				return hr;
			}
		}

	}
	_variant_t vtParamIDType((long)_idQuestionType);
	hr = m_pDb->AddParameter(L"ID_TYPE", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(long),&vtParamIDType);
	if(FAILED(hr))
	{
		return hr;
	}

	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(m_uObjID),&vtParam);
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CYDQuestionVault::CreateConditionSQL(CString _strDBName,
						   std::list<CPropQueryContidition*> *_lstCondition,
						   CString &_strSQL)
{
	HRESULT hr = E_FAIL;
	_strSQL = _T("SELECT A.OBJID,B.LINKID FROM ");
	_strSQL += _T(" ( SELECT OBJID FROM ");
	_strSQL += _strDBName;
	BOOL bFirst = TRUE;
	if(_lstCondition != NULL)
	{
		for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
			itr != _lstCondition->end();++itr)
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
	}
	
	_strSQL += _T(") A RIGHT JOIN ");
	_strSQL += _T(" ( select OBJID AS LINKID,ID_B FROM ");
	_strSQL += DB_VAULTQUESTION;
	_strSQL += _T(" where ID_TYPE = ? AND ID_A = ?)  B ");
	_strSQL += _T(" On A.OBJID = B.ID_B ");
	_strSQL += _T(" ORDER BY OBJID DESC ");

	return S_OK;
}

HRESULT CYDQuestionVault::ExeDBCreateQuestion(QUESTION_TYPE _qType,
							std::list<CYDQuestionRef*>* _sub_question,
							std::list<CYDLinkRef*>* _sub_link /*= NULL*/	)
{
	HRESULT hr = E_FAIL;
	int index = 0;
	ASSERT(m_pQuestionRecord);
	while(!m_pQuestionRecord->adoEOF && index++ < QUESTION_PAGE_COUNT)
	{
		CString strObjID;
		FieldsPtr fields = m_pQuestionRecord->GetFields();
		CComVariant valObjID = fields->GetItem(_variant_t(_T("OBJID")))->GetValue();

		OBJID uID = CDataHandler::VariantToLong(valObjID);
		if(uID != 0)
		{
			CYDQuestionRef* pQRef = NULL;
			if(_qType == QUESTION_CHOICE)
			{
				pQRef = new CYDChoiceQuestionRef(m_pDb);
			}
			else if(_qType == QUESTION_ARTICLE)
			{
				pQRef = new CYDArticleQuestionRef(m_pDb);
			}
			else
			{
				ASSERT(FALSE);
			}
			ASSERT(pQRef);
			hr = pQRef->SetID(uID);
			if (FAILED(hr))
			{
				return hr;
			}
			_sub_question->push_back(pQRef);
			if(_sub_link != NULL)
			{
				CComVariant valLinkID = fields->GetItem(_variant_t(_T("LINKID")))->GetValue();
				OBJID uLinkID = CDataHandler::VariantToLong(valLinkID);
				CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_VAULTQUESTION);
				hr = pLinkRef->SetID(uLinkID);
				if (FAILED(hr))
				{
					return hr;
				}
				_sub_link->push_back(pLinkRef);
			}
		}
		m_pQuestionRecord->MoveNext();
	}
	return S_OK;
}

HRESULT CYDQuestionVault::CurQuestionIsEof(BOOL &_bIsEof)
{
	HRESULT hr = E_FAIL;
	if(m_pQuestionRecord == NULL ||
		m_pQuestionRecord->State != adStateOpen)
	{
		_bIsEof = TRUE;
		return S_FALSE;
	}
	_bIsEof = m_pQuestionRecord->adoEOF;
	return S_OK;
}