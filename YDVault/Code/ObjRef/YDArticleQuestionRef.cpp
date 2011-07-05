#include "StdAfx.h"
#include "YDArticleQuestionRef.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "YDChoiceQuestionref.h"
#include "../Base/AutoClean.h"
#include "YDLinkRef.h"
#include "YdObjWrapper.h"
#include "../FtpBase/FTPDownLoad.h"


CYDArticleQuestionRef::CYDArticleQuestionRef(CDatabaseEx* pDb)
:CYDQuestionRef(pDb)
{
	m_strDBName = DB_ARTICLEQUESTION;
	AddPropDef(FIELD_ARTICLEQUESTION_ARTICLE,VT_BLOB,_T(""));
	AddPropDef(FIELD_ARTICLEQUESTION_TYPEID,VT_I4);
	AddPropDef(FIELD_ARTICLEQUESTION_HARDLEVEL,VT_I4);
	AddPropDef(FIELD_ARTICLEQUESTION_USEDCOUNT, VT_I4, _T("0"));
	AddPropDef(FIELD_ARTICLEQUESTION_CREATOR,VT_BSTR);
	AddPropDef(FIELD_ARTICLEQUESTION_CREATEDATE,VT_DATE);
	AddPropDef(FIELD_ARTICLEQUESTION_QNUM, VT_I4, _T("1"));
	AddPropDef(FIELD_ARTICLEQUESTION_IMAGETITLE,VT_BLOB_OBJECT);
	AddPropDef(FIELD_ARTICLEQUESTION_TITLEMODE, VT_I4, _T("1"));

	CreateFactorInfoItem();

	for (int i = 1; i < 101; ++i)
	{
		CString strSeq;
		strSeq.Format(L" (%d) ", i);
		
		m_LstSequence.push_back(strSeq);
	}
}

CYDArticleQuestionRef::~CYDArticleQuestionRef(void)
{
}

HRESULT CYDArticleQuestionRef::GetChildQueston(std::list<CYDQuestionRef*>* _lstQuestion,
						std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_ARTICLEQCHOICEQ;
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
		CYDChoiceQuestionRef* pChild = new CYDChoiceQuestionRef(m_pDb);
		_lstQuestion->push_back(pChild);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			//失败清除
			CListAutoClean<CYDQuestionRef> clean(*_lstQuestion);
			return hr;
		}
		if(_sub_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_ARTICLEQCHOICEQ);
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

HRESULT CYDArticleQuestionRef::GetLabel(CString* pStr)
{
	HRESULT hr = E_FAIL;
	CComVariant valArticle;
	hr = GetPropVal(L"ARTICLE",&valArticle);
	if(FAILED(hr))
	{
		return hr;
	}
	*pStr = CDataHandler::VariantToString(valArticle);
	return S_OK;
}

HRESULT CYDArticleQuestionRef::GetPropVal(BSTR _bstrPropName,VARIANT* _val)
{
	HRESULT hr = E_FAIL;
	hr = CYDQuestionRef::GetPropVal(_bstrPropName,_val);
	if(FAILED(hr))
	{
		return hr;
	}
	if(CDataHandler::BSTRCompare(_bstrPropName,L"ARTICLE") == 0)
	{
		//要将Blob字段转化为字符串
		if(_val->vt == (VT_ARRAY   |   VT_UI1))
		{
			char   *pBuf   =   NULL;   
			SafeArrayAccessData(_val->parray,(void   **)&pBuf);                             ///得到指向数据的指针   
			/*****在这里我们可以对pBuf中的数据进行处理*****/   
			TCHAR *pBuffer = NULL;
			DWORD dwNum = MultiByteToWideChar (::GetACP (), 0, pBuf, -1, NULL, 0);
			pBuffer = new TCHAR [dwNum + 1];			
#ifdef _UNICODE
			wmemset(pBuffer, 0, dwNum + 1);
#else
			memset(pBuffer, 0, dwNum + 1);
#endif
			MultiByteToWideChar (::GetACP (), 0, pBuf, -1, pBuffer, dwNum);
			CString str;
			str.Format(_T("%s"),pBuffer);
			delete pBuffer;
			pBuffer = NULL;
			SafeArrayUnaccessData   (_val->parray);   
			CDataHandler::StringToVariant(str,VT_BSTR,_val);
		}
	

	}
	return S_OK;
}


HRESULT CYDArticleQuestionRef::Remove()
{
	HRESULT hr = E_FAIL;
	std::list<CYdObjWrapper*> lstDelObjWrapper;
	CListAutoClean<CYdObjWrapper> clr(lstDelObjWrapper);
	{
		//删除关联的ChoiceQuestion
		std::list<CYDQuestionRef*> lstQuestion;
		std::list<CYDLinkRef*> sub_link;
		hr = GetChildQueston(&lstQuestion,&sub_link);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstQuestion.size() == sub_link.size());
		std::list<CYDQuestionRef*>::iterator itr = lstQuestion.begin();
		std::list<CYDLinkRef*>::iterator itrLink = sub_link.begin();
		for (;itr != lstQuestion.end() && itrLink != sub_link.end();
			++itr,++itrLink)
		{
			CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
			pObjWrapper->m_state = S_NEED_DELETE_BOTH;
			pObjWrapper->m_pObjRef = (*itr);
			pObjWrapper->m_pLinkRef = (*itrLink);
			hr = pObjWrapper->Remove();
			if(FAILED(hr))
			{
				return hr;
			}
			lstDelObjWrapper.push_back(pObjWrapper);
		}
		
	}

	
	hr = CYDQuestionRef::Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

bool CYDArticleQuestionRef::ToString(CString* pStr)
{
	int index = 1;
	HRESULT hr = ToString(&index, pStr);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

HRESULT CYDArticleQuestionRef::WriteArticle(int* pIndex, CString& strArticle, CWordAppHelper* pWord)
{
	HRESULT hr = E_FAIL;
	std::list<CString>::const_iterator itr = m_LstSequence.begin();
	int nBegin = 0;
	for (; itr != m_LstSequence.end(); ++itr)
	{
		int nPos = strArticle.Find(*itr);
		int nCount;
		if (nPos < 0)
		{
			nCount = strArticle.GetLength() - nBegin;
		}
		else
		{
			nCount = nPos - nBegin;
		}
		CString str = strArticle.Mid(nBegin, nCount);
		pWord->Write(str);
		if (nPos >= 0)
		{
			pWord->StartUnline();
			CString strUnline;
			strUnline.Format(L" %d ", *pIndex);
			pWord->Write(strUnline);
			pWord->EndUnline();
		}
		
		if (nPos < 0)
		{
			break;
		}
		(*pIndex)++;
		nBegin = nPos + (*itr).GetLength();
	}
	return S_OK;
}

HRESULT CYDArticleQuestionRef::ToString(int* pIndex, CString* pStr)
{
	CComVariant varAritcle;
	HRESULT hr = GetPropVal(L"ARTICLE", &varAritcle);
	if (FAILED(hr))
	{
		return false;
	}
	CString strArticle = CDataHandler::VariantToString(varAritcle);
	*pStr = strArticle;
	*pStr += L"\r\n";

	std::list<CYDQuestionRef*> lstChildQuestions;
	CListAutoClean<CYDQuestionRef> clean1(lstChildQuestions);
	hr = GetChildQueston(&lstChildQuestions, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	std::list<CYDQuestionRef*>::const_iterator itrQ = lstChildQuestions.begin();
	for (; itrQ != lstChildQuestions.end(); ++itrQ)
	{
		CString strQ;
		(*itrQ)->ToString(&strQ);
		CString strIndex;
		strIndex.Format(L"%d. ", *pIndex);
		*pStr += strIndex;
		*pStr += strQ;
		*pStr += L"\r\n";

		(*pIndex)++;
	}

	return S_OK;
}

HRESULT CYDArticleQuestionRef::PersistToMSWord(int* pIndex, 
											   CWordAppHelper* pWord, 
											   CWordAppHelper* pWordAnswer,
											   const CString& strFolder)
{
	HRESULT hr = E_FAIL;

	QTYPE qType;
	GetQuestionType(&qType);

	if (qType == QTYPE_LISTENINGSHORT ||
		qType == QTYPE_LISTENINGLONG ||
		qType == QTYPE_LISTENINGCOMPREHENSION ||
		qType == QTYPE_LISTENINGVOCABULARY)
	{
		return PersistListioningToMSWord(pIndex, pWord, pWordAnswer, strFolder, qType);
	}
	else if (qType == QTYPE_CLOZE ||
			 qType == QTYPE_READINGVOCABULARY ||
			 qType == QTYPE_ERRORCORRECTION)
	{
		return PersistUnlineTypeToMSWord(pIndex, pWord, pWordAnswer, strFolder, qType);
	}
	else
	{
		return PersistNormalTypeToMSWord(pIndex, pWord, pWordAnswer, strFolder, qType);
	}
		
	return S_OK;
}
HRESULT CYDArticleQuestionRef::PersistNormalTypeToMSWord(int* pIndex, 
														 CWordAppHelper* pWord, 
														 CWordAppHelper* pWordAnswer, 
														 const CString& strFolder, 
														 QTYPE qType)
{
	HRESULT hr = E_FAIL;

	CComVariant varAritcle;
	hr = GetPropVal(L"ARTICLE", &varAritcle);
	if (FAILED(hr))
	{
		return false;
	}
	CString strArticle = CDataHandler::VariantToString(varAritcle);
	strArticle += L"\r\n";
	
	hr = pWord->Write(strArticle);
	if (FAILED(hr))
	{
		return hr;
	}

	std::list<CYDQuestionRef*> lstChildQuestions;
	CListAutoClean<CYDQuestionRef> clean1(lstChildQuestions);
	hr = GetChildQueston(&lstChildQuestions, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	CString strChild;
	std::list<CYDQuestionRef*>::const_iterator itrQ = lstChildQuestions.begin();
	for (; itrQ != lstChildQuestions.end(); ++itrQ)
	{
		int nAnswer = *pIndex;
		CString strQ;
		(*itrQ)->ToString(&strQ);
		
		CString strIndex;
		strIndex.Format(L"%d. ", *pIndex);
		strChild += strIndex;
		(*pIndex)++;
		strChild += strQ;
		strChild += L"\r\n";

		//写答案
		std::list<CString> lstAnswer;
		(*itrQ)->GetAnswerList(&lstAnswer);
		for (std::list<CString>::const_iterator itr = lstAnswer.begin();
			itr != lstAnswer.end(); ++itr)
		{
			CString strAnswer;
			CString strNO;
			strNO.Format(L"%d)", nAnswer);
			strAnswer = strNO + (*itr);
			strAnswer += L"\r\n";
			hr = pWordAnswer->Write(strAnswer);
			if (FAILED(hr))
			{
				return hr;
			}
			nAnswer++;
		}

	}

	hr = pWord->Write(strChild);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CYDArticleQuestionRef::GetUnderLineNum(int* pNum)
{
	CComVariant var;
	HRESULT hr = GetPropVal(L"ARTICLE", &var);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strArticle = CDataHandler::VariantToString(var);
	std::list<CString>::const_iterator itr = m_LstSequence.begin();
	int nBegin = 0;
	int nCount = 0;
	for (; itr != m_LstSequence.end(); ++itr)
	{
		int nPos = strArticle.Find(*itr);
		
		if (nPos < 0)
		{
			break;
		}
		++nCount;
		nBegin = nPos + (*itr).GetLength();
	}
	*pNum = nCount;
	return S_OK;
}
//选词填空，完形填空，改错

HRESULT CYDArticleQuestionRef::PersistUnlineTypeToMSWord(int* pIndex, 
														  CWordAppHelper* pWord, 
														  CWordAppHelper* pWordAnswer, 
														  const CString& strFolder, 
														  QTYPE qType)
{
	HRESULT hr = E_FAIL;

	CComVariant varAritcle;
	hr = GetPropVal(L"ARTICLE", &varAritcle);
	if (FAILED(hr))
	{
		return false;
	}
	CString strArticle = CDataHandler::VariantToString(varAritcle);
	strArticle += L"\r\n";
	int nIndex = *pIndex;
	
	hr = WriteArticle(pIndex, strArticle, pWord);
	if (FAILED(hr))
	{
		return hr;
	}
	
	std::list<CYDQuestionRef*> lstChildQuestions;
	CListAutoClean<CYDQuestionRef> clean1(lstChildQuestions);
	hr = GetChildQueston(&lstChildQuestions, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	CString strChild;
	std::list<CYDQuestionRef*>::const_iterator itrQ = lstChildQuestions.begin();
	for (; itrQ != lstChildQuestions.end(); ++itrQ)
	{
		int nAnswer = nIndex;
		if (qType != QTYPE_ERRORCORRECTION)//改错没有选项
		{
			CString strQ;
			(*itrQ)->ToString(&strQ);
			CString strIndex;
			strIndex.Format(L"%d. ", nIndex);
			strChild += strIndex;
			nIndex++;
			strChild += strQ;
			strChild += L"\r\n";
		}

		//写答案
		std::list<CString> lstAnswer;
		(*itrQ)->GetAnswerList(&lstAnswer);
		for (std::list<CString>::const_iterator itr = lstAnswer.begin();
			itr != lstAnswer.end(); ++itr)
		{
			CString strAnswer;
			CString strNO;
			strNO.Format(L"%d)", nAnswer);
			strAnswer = strNO + (*itr);
			strAnswer += L"\r\n";
			hr = pWordAnswer->Write(strAnswer);
			if (FAILED(hr))
			{
				return hr;
			}
			nAnswer++;
		}

	}

	hr = pWord->Write(strChild);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CYDArticleQuestionRef::PersistListioningToMSWord(int* pIndex, 
														  CWordAppHelper* pWord, 
														  CWordAppHelper* pWordAnswer,
														  const CString& strFolder,
														  QTYPE qType)
{
	HRESULT hr = E_FAIL;
	//将音频插入word
	std::list<CYDMediaRef*> lstMedia;
	CListAutoClean<CYDMediaRef> clr(lstMedia);
	hr = GetAllMedia(&lstMedia, NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDMediaRef*>::const_iterator itrMedia = lstMedia.begin();
	for (; itrMedia != lstMedia.end(); ++itrMedia)
	{
		CString strVaultName;
		(*itrMedia)->CreateVaultName(strVaultName);
		CString strIndex;
		strIndex.Format(L"%d_", *pIndex);
		CString strLabel;
		(*itrMedia)->GetLabel(&strLabel);
		CString strName = strIndex + strLabel;
		CString strLocal = strFolder + L"/" + strName;
		hr = m_pFtp->DownLoad(strLocal, strVaultName);
		if (FAILED(hr))
		{
			CString strFormat = L"听力文件<%s>下载失败!";
			CString strInfo;
			strInfo.Format(strFormat, strLocal);
			AfxMessageBox(strInfo);
			return E_FAIL;
		}
		pWord->AddOleObject(strLocal);
		CString s = L"\r\n";
		pWord->Write(s);
	}
	CComVariant varAritcle;
	hr = GetPropVal(L"ARTICLE", &varAritcle);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strArticle = CDataHandler::VariantToString(varAritcle);
	strArticle += L"\r\n";
	int nIndex = *pIndex;
	//听力填空写原文，加下划线
	if (qType == QTYPE_LISTENINGVOCABULARY)
	{
		hr = WriteArticle(pIndex, strArticle, pWord);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	//听力把参考原文写进答案
	if (qType == QTYPE_LISTENINGSHORT ||
		qType == QTYPE_LISTENINGLONG ||
		qType == QTYPE_LISTENINGCOMPREHENSION)
	{
		CString strAnswer;
		strAnswer += strArticle;
		strAnswer += L"\r\n";
		hr = pWordAnswer->Write(strAnswer);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	
	std::list<CYDQuestionRef*> lstChildQuestions;
	CListAutoClean<CYDQuestionRef> clean1(lstChildQuestions);
	hr = GetChildQueston(&lstChildQuestions, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	CString strChild;
	std::list<CYDQuestionRef*>::const_iterator itrQ = lstChildQuestions.begin();
	for (; itrQ != lstChildQuestions.end(); ++itrQ)
	{
		int nAnswer = nIndex;
		CString strQ;
		(*itrQ)->ToString(&strQ);
		if (qType != QTYPE_LISTENINGVOCABULARY)//非听力填空有选项
		{
			CString strIndex;
			strIndex.Format(L"%d. ", nIndex);
			strChild += strIndex;

			strChild += strQ;
			strChild += L"\r\n";
			nIndex++;
			
			(*pIndex)++;
		}

		//写答案
		std::list<CString> lstAnswer;
		(*itrQ)->GetAnswerList(&lstAnswer);
		for (std::list<CString>::const_iterator itr = lstAnswer.begin();
			itr != lstAnswer.end(); ++itr)
		{
			CString strAnswer;
			CString strNO;
			strNO.Format(L"%d)", nAnswer);
			strAnswer = strNO + (*itr);
			strAnswer += L"\r\n";
			hr = pWordAnswer->Write(strAnswer);
			if (FAILED(hr))
			{
				return hr;
			}
			nAnswer++;
		}

	}

	hr = pWord->Write(strChild);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CYDArticleQuestionRef::GetStandardAnswer(CString &_str)
{
	HRESULT hr = E_FAIL;
	std::list<CString> lstStdAnswer;
	hr = GetStandardAnswer(lstStdAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	int index = 1;
	for(std::list<CString>::const_iterator itr = lstStdAnswer.begin();
		itr != lstStdAnswer.end();++itr,++index)
	{
		CString strItrAnswer;
		strItrAnswer.Format(_T("%d:%s"),index,*itr);
		_str += strItrAnswer;
	}

	return S_OK;
}

HRESULT CYDArticleQuestionRef::GetStandardAnswer(std::list<CString> &_lstStdAnswer)
{
	HRESULT hr = E_FAIL;
	std::list<CYDQuestionRef*> lstQuestion;
	CListAutoClean<CYDQuestionRef> clr(lstQuestion);
	hr = GetChildQueston(&lstQuestion);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDQuestionRef*>::const_iterator itr = lstQuestion.begin();
		itr != lstQuestion.end();++itr)
	{
		CString strAnswer;
		hr = (*itr)->GetStandardAnswer(strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		_lstStdAnswer.push_back(strAnswer);
	}
	return S_OK;
}