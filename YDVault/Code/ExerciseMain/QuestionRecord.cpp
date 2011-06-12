#include "StdAfx.h"
#include "QuestionRecord.h"
#include "../\Base\AutoClean.h"
#include "QuestionRecordStruct.h"
#include "../Base\FilePathHelper.h"
#include "../ObjRef\YDQuestionRef.h"
#include "../Base\DataHandler.h"

CQuestionRecord::CQuestionRecord(CString _strFileName)
{
	m_strFileName = _strFileName;
	m_iCurNo = -1;
}

CQuestionRecord::~CQuestionRecord(void)
{
	Clear();
}

HRESULT CQuestionRecord::Clear()
{
	CListAutoClean<CQuestionRecordStruct> clr(m_lstQuestionStruct);
	return S_OK;
}

HRESULT CQuestionRecord::GetFilePath(CString &_strPath)
{
	HRESULT hr = E_FAIL;
	_strPath = g_LogPath;
	_strPath += TEXT("\\");
	_strPath += m_strFileName;
	return S_OK;
}

HRESULT CQuestionRecord::AddRecord(UINT _uQNo, CYDQuestionRef* _pQestion, CQuestionRecordStruct** ppNewRecordStruct)
{
	HRESULT hr = E_FAIL;
	CQuestionRecordStruct* pRecordStruct = new CQuestionRecordStruct();
	pRecordStruct->m_QNo = _uQNo;
	CComVariant valType;
	hr = _pQestion->GetPropVal(L"TYPEID",&valType);
	if(FAILED(hr))
	{
		return hr;
	}
	pRecordStruct->m_QTypeID = (UINT)CDataHandler::VariantToLong(valType);
	hr = _pQestion->GetID(&(pRecordStruct->m_uObjID));
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = _pQestion->GetPropVal(L"ANSWER",(pRecordStruct->m_strStdAnswer));
	if(FAILED(hr))
	{
		return hr;
	}
	pRecordStruct->m_pQuestion = _pQestion;
	CComVariant valMark;
	_pQestion->GetPropVal(L"Mark",&valMark);
	pRecordStruct->m_fMark = (float)CDataHandler::VariantToLong(valMark);
	*ppNewRecordStruct = pRecordStruct;
	m_lstQuestionStruct.push_back(*ppNewRecordStruct);	
	return S_OK;
}

HRESULT CQuestionRecord::ComputeMark(double* _dbMark, int* _accuracy)
{
	HRESULT hr = E_FAIL;
	*_dbMark = 0.0;
	int allcount = 0;
	int yescount = 0;
	for(std::list<CQuestionRecordStruct*>::const_iterator itr = m_lstQuestionStruct.begin();
		itr != m_lstQuestionStruct.end();++itr)
	{
		double dbItrMark = 0.0;
		hr = (*itr)->ComputeMark(&dbItrMark);
		if(FAILED(hr))
		{
			return hr;
		}
		allcount += (*itr)->m_answercount;
		yescount += (*itr)->m_yescount;
		*_dbMark += dbItrMark;
	}
	if (allcount != 0)
	{
		*_accuracy = (yescount*100)/allcount;
	}
	return S_OK;
}


HRESULT CQuestionRecord::SetFileName(CString& _strFileName,BOOL bRenameFile /*= FALSE*/)
{
	HRESULT hr = E_FAIL;
	CString strOldFile;
	if(bRenameFile)
	{
		hr = GetFilePath(strOldFile);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	m_strFileName = _strFileName;
	if(bRenameFile)
	{
		CString strNewFile;
		hr = GetFilePath(strNewFile);
		if(FAILED(hr))
		{
			return hr;
		}
		CFile::Rename(strOldFile,strNewFile);
	}
	return S_OK;
}

HRESULT CQuestionRecord::GetFileName(CString& _strFileName)
{
	HRESULT hr = E_FAIL;
	_strFileName = m_strFileName;
	return S_OK;
}