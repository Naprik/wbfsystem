#include "StdAfx.h"
#include "PaperAnsysHelper.h"
#include "../ObjRef/YDPaper.h"
#include "../YDExamObjRef/YDStuMark.h"
#include "../Base/AutoClean.h"
#include "PaperErrorResult.h"

CPaperAnsysHelper::CPaperAnsysHelper(void)
{
}

CPaperAnsysHelper::~CPaperAnsysHelper(void)
{
}

HRESULT CPaperAnsysHelper::Ansys(OBJID paperID, 
								 std::list<CYDObjectRef*>& lstMarks, 
								 std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAnswer;
	CListAutoClean<CYDObjectRef> clrAns(lstAnswer);
	hr = CreateAnsysHelperResultByPaperID(paperID,lstAnswer,mapRes);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrAns = lstAnswer.begin();
	for (; itrAns != lstAnswer.end(); ++itrAns)
	{
		hr = AnsysOne(*itrAns, lstMarks, mapRes);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CPaperAnsysHelper::Ansys(OBJID paperID, 
						std::list<CString>& lstStrAnswer,  
					 std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAnswer;
	CListAutoClean<CYDObjectRef> clrAns(lstAnswer);
	hr = CreateAnsysHelperResultByPaperID(paperID,lstAnswer,mapRes);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrAns = lstAnswer.begin();
	for (; itrAns != lstAnswer.end(); ++itrAns)
	{
		hr = AnsysOne(*itrAns, lstStrAnswer, mapRes);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CPaperAnsysHelper::CreateAnsysHelperResultByPaperID(OBJID paperID, 
															std::list<CYDObjectRef*> &lstAnswer,
										std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDPaper paper(pDB);
	paper.SetID(paperID);
	
	hr = paper.GetStdAnswer(&lstAnswer);
	if (FAILED(hr))
	{
		return hr;
	}
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
	if(begin1 >0 && end1 > 0)
	{
		for (int i=begin1; i<=end1; ++i)
		{
			CPaperAnsysHelperResult* pRes = new CPaperAnsysHelperResult;
			pRes->m_iError = 0;
			pRes->m_iRight = 0;
			TCHAR ch = L'A';
			for (int j=0; j<num1; ++j, ++ch)
			{
				pRes->m_mapAnswer.insert(std::map<CString, UINT>::value_type(CString(ch), 0));
			}
			mapRes.insert(std::map<UINT, CPaperAnsysHelperResult*>::value_type(i, pRes));
		}
	}
	if(begin2 >0 && end2 >0 )
	{
		for (int i=begin2; i<=end2; ++i)
		{
			CPaperAnsysHelperResult* pRes = new CPaperAnsysHelperResult;
			pRes->m_iError = 0;
			pRes->m_iRight = 0;
			TCHAR ch = L'A';
			for (int j=0; j<num2; ++j, ++ch)
			{
				pRes->m_mapAnswer.insert(std::map<CString, UINT>::value_type(CString(ch), 0));
			}
			mapRes.insert(std::map<UINT, CPaperAnsysHelperResult*>::value_type(i, pRes));
		}
	}

	if(begin3 > 0 && end3 > 0 )
	{
		for (int i=begin3; i<=end3; ++i)
		{
			CPaperAnsysHelperResult* pRes = new CPaperAnsysHelperResult;
			pRes->m_iError = 0;
			pRes->m_iRight = 0;
			TCHAR ch = L'A';
			for (int j=0; j<num3; ++j, ++ch)
			{
				pRes->m_mapAnswer.insert(std::map<CString, UINT>::value_type(CString(ch), 0));
			}
			mapRes.insert(std::map<UINT, CPaperAnsysHelperResult*>::value_type(i, pRes));
		}
	}
	return S_OK;
}

HRESULT CPaperAnsysHelper::ErrorReport(std::list<CYDObjectRef*>& lstMarks,
					std::list<CPaperErrorResult*>& _lstErrorResult)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	for (std::list<CYDObjectRef*>::const_iterator itrMarks = lstMarks.begin();
		itrMarks != lstMarks.end(); ++itrMarks)
	{
		long paperID = ID_EMPTY;
		(*itrMarks)->GetPropVal(FIELD_YDSTUMARK_PAPERID, &paperID);
		CPaperErrorResult* pResult = NULL;
		for(std::list<CPaperErrorResult*>::const_iterator itrReuslt = _lstErrorResult.begin();
			itrReuslt != _lstErrorResult.end();++itrReuslt)
		{
			CYDPaper* pItrPaper = (*itrReuslt)->m_pPaper;
			ASSERT(pItrPaper);
			OBJID IDItrPaper = ID_EMPTY;
			hr = pItrPaper->GetID(&IDItrPaper);
			if (FAILED(hr))
			{
				return hr;
			}
			if(IDItrPaper == paperID)
			{
				pResult = *itrReuslt;
				break;
			}
		}
		if(pResult == NULL)
		{
			//说明没有找到，要新建一个插入到列表中
			pResult = new CPaperErrorResult();
			_lstErrorResult.push_back(pResult);
			CYDPaper* pPaper = new CYDPaper(pDB);
			pPaper->SetID(paperID);
			pResult->m_pPaper = pPaper;
		}
		ASSERT(pResult);
		//判断当前学生是答案是否有错误
		BOOL bHasError = FALSE;
		
		std::list<CYDObjectRef*> lstAnswer;
		CListAutoClean<CYDObjectRef> clrAns(lstAnswer);
		hr = pResult->m_pPaper->GetStdAnswer(&lstAnswer);
		if (FAILED(hr))
		{
			return hr;
		}
		if (lstAnswer.size() <= 0)
		{
			continue;
		}
		std::list<UINT> lstQNo;
		std::list<CYDObjectRef*>::const_iterator itrAns = lstAnswer.begin();
		for (; itrAns != lstAnswer.end(); ++itrAns)
		{
			long lNO;
			(*itrAns)->GetPropVal(FIELD_YDANSWER_NO, &lNO);
			CComVariant varMark;
			(*itrAns)->GetPropVal(FIELD_YDANSWER_MARK, &varMark);
			double dMark = (double)_variant_t(varMark);
			CString strAnswer;
			(*itrAns)->GetPropVal(FIELD_YDANSWER_ANSWER, strAnswer);

			CYDStuMark* pMark = dynamic_cast<CYDStuMark*>(*itrMarks);
			std::map<int, CString> mapAnswer;
			hr = pMark->TransferAnswer(&mapAnswer);
			if (FAILED(hr))
			{
				return hr;
			}
			std::map<int, CString>::iterator itrMap = mapAnswer.find(lNO);
			if (itrMap != mapAnswer.end())
			{
				if (itrMap->second.CompareNoCase(strAnswer) != 0)
				{
					bHasError = TRUE;
					//判断题号lNO是否在列表中
					BOOL bFind = FALSE;
					for(std::list<UINT>::const_iterator itrErrorItemNo = pResult->m_lstErrorItemNo.begin();
						itrErrorItemNo != pResult->m_lstErrorItemNo.end();++itrErrorItemNo)
					{
						if(lNO == (*itrErrorItemNo))
						{
							bFind = TRUE;
							break;
						}
					}
					//在错误号列表中插入一个题号
					if(!bFind)
					{
						pResult->m_lstErrorItemNo.push_back(lNO);
					}
					
				}
			}
		}
		if(bHasError)
		{
			//要将当前学生插入到列表中
			CYDStuMark* pMark = dynamic_cast<CYDStuMark*>(*itrMarks);
			pResult->m_lstStuMark.push_back(pMark);
		}
	}
	//将结果集中的错误题号排序一下
	for(std::list<CPaperErrorResult*>::const_iterator itr = _lstErrorResult.begin();
		itr != _lstErrorResult.end();++itr)
	{
		(*itr)->m_lstErrorItemNo.sort();
	}
	return S_OK;
}

HRESULT CPaperAnsysHelper::AnsysOne(CYDObjectRef* pObjAnswer, 
									 std::list<CYDObjectRef*>& lstMarks, 
									 std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*>::const_iterator itrMark = lstMarks.begin();
	for (; itrMark != lstMarks.end(); ++itrMark)
	{
		CYDStuMark* pMark = dynamic_cast<CYDStuMark*>(*itrMark);
		std::map<int, CString> mapAnswer;
		hr = pMark->TransferAnswer(&mapAnswer);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = AnsysOne(pObjAnswer,mapAnswer,mapRes);
		if(FAILED(hr))
		{
			return hr;
		}

	}

	return S_OK;
}

HRESULT CPaperAnsysHelper::AnsysOne(CYDObjectRef* pObjAnswer, 
				std::list<CString>& lstStrAnswer,  
				 std::map<UINT, CPaperAnsysHelperResult*>& mapRes)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	for (std::list<CString>::const_iterator itr = lstStrAnswer.begin();
		itr != lstStrAnswer.end(); ++itr)
	{
	
		CYDStuMark YdStuMak(pDB);
		CComVariant valAnswer(*itr);
		hr =  YdStuMak.SetPropVal(FIELD_YDSTUMARK_ANSWER,&valAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		std::map<int, CString> mapAnswer;
		hr = YdStuMak.TransferAnswerByString(&mapAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = AnsysOne(pObjAnswer,mapAnswer,mapRes);
		if(FAILED(hr))
		{
			return hr;
		}

	}

	return S_OK;
}

HRESULT CPaperAnsysHelper::AnsysOne(CYDObjectRef* pObjAnswer,//标准答案
				 std::map<int, CString> mapAnswer,/*题号与答案的映射*/
				 std::map<UINT, CPaperAnsysHelperResult*>& mapRes/*分析的结果*/)
{
	HRESULT hr = E_FAIL;
	long lNO;
	pObjAnswer->GetPropVal(FIELD_YDANSWER_NO, &lNO);
	CComVariant varMark;
	pObjAnswer->GetPropVal(FIELD_YDANSWER_MARK, &varMark);
	double dMark = (double)_variant_t(varMark);
	CString strStandAnswer;
	pObjAnswer->GetPropVal(FIELD_YDANSWER_ANSWER, strStandAnswer);
	std::map<int, CString>::iterator itrMap = mapAnswer.find(lNO);
	if (itrMap != mapAnswer.end())
	{
		std::map<UINT, CPaperAnsysHelperResult*>::iterator itrRes = mapRes.find(lNO);
		CPaperAnsysHelperResult* pRes = NULL;
		if (itrRes == mapRes.end())
		{
			CPaperAnsysHelperResult* pRes = new CPaperAnsysHelperResult();
			mapRes.insert(std::map<UINT, CPaperAnsysHelperResult*>::iterator::value_type(lNO, pRes));
		}
		else
		{
			pRes = itrRes->second;
		}
		pRes->m_strRightAnswer = strStandAnswer;
		CString strMapAnswer = itrMap->second;
		if(strMapAnswer.IsEmpty())
		{
			strMapAnswer = YD_NOT_FILL_ANSWER;
		}
		if (strMapAnswer.CompareNoCase(strStandAnswer) == 0)
		{
			pRes->m_iRight++;
		}
		else
		{
			pRes->m_iError++;
		}
		
		BOOL bFind = FALSE;
		for (std::map<CString, UINT>::iterator itrAnsNum = pRes->m_mapAnswer.begin(); 
			itrAnsNum != pRes->m_mapAnswer.end(); ++itrAnsNum)
		{
			if (strMapAnswer.CompareNoCase(itrAnsNum->first) == 0)
			{
				(itrAnsNum->second)++;
				bFind = TRUE;
				break;
			}
		}
		if(!bFind)
		{
			pRes->m_mapAnswer.insert(std::map<CString, UINT>::value_type(strMapAnswer, 1));
		}
	}
	else
	{
		//当前这道题目没有填写答案
		std::map<UINT, CPaperAnsysHelperResult*>::iterator itrRes = mapRes.find(lNO);
		CPaperAnsysHelperResult* pRes = NULL;
		if (itrRes == mapRes.end())
		{
			CPaperAnsysHelperResult* pRes = new CPaperAnsysHelperResult();
			mapRes.insert(std::map<UINT, CPaperAnsysHelperResult*>::iterator::value_type(lNO, pRes));
		}
		else
		{
			pRes = itrRes->second;
		}
		pRes->m_strRightAnswer = strStandAnswer;
		ASSERT(pRes);
		pRes->m_iError++;
		BOOL bFind = FALSE;
		CString strMapAnswer = YD_NOT_FILL_ANSWER;
		for (std::map<CString, UINT>::iterator itrAnsNum = pRes->m_mapAnswer.begin(); 
			itrAnsNum != pRes->m_mapAnswer.end(); ++itrAnsNum)
		{
			if (strMapAnswer.CompareNoCase(itrAnsNum->first) == 0)
			{
				(itrAnsNum->second)++;
				bFind = TRUE;
				break;
			}
		}
		if(!bFind)
		{
			pRes->m_mapAnswer.insert(std::map<CString, UINT>::value_type(strMapAnswer, 1));
		}
	}
	return S_OK;
}