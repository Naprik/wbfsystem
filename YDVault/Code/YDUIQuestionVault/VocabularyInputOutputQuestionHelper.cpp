#include "StdAfx.h"
#include "VocabularyInputOutputQuestionHelper.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "DlgVocabularyInputPreview.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef\YDChoiceQuestionRef.h"
#include "../ObjHelper\FactorInfoHelper.h"

HRESULT CVocabularyQuestion::Load(Paragraphs &_paragraphs,long _index)
{
	HRESULT hr = E_FAIL;
	std::list<CString> lstStr;
	long lCount = _paragraphs.GetCount();
	while(_index <= lCount)
	{
		// 得到一段 
		Paragraph paragraph; 
		paragraph = _paragraphs.Item( _index );

		// 得到一段的文本 
		Range range = paragraph.GetRange(); 
		CString strRangeText = range.GetText();
		range.ReleaseDispatch();
		lstStr.push_back(strRangeText);

		//释放对象 
		paragraph.ReleaseDispatch();

	}
	return S_OK;
}


CVocabularyInputOutputQuestionHelper::CVocabularyInputOutputQuestionHelper(CYDObjectRef* _pVault,CYDObjectRef* _pType)
	:CInputOutputQuestionHelper(_pVault,_pType)
{
}


CVocabularyInputOutputQuestionHelper::~CVocabularyInputOutputQuestionHelper(void)
{
}

HRESULT CVocabularyInputOutputQuestionHelper::ExeInputFile(CString _strFile)
{
	HRESULT hr = E_FAIL;
	hr = CreateWord(_strFile);
	if(FAILED(hr))
	{
		return hr;
	}
	long paCounst = m_paragraphs.GetCount(); 
	std::list<CString> lstStr;
	// 将所有段中的每一段都存在数组里 
	for(long lc = 1 ; lc < paCounst+1 ; lc++ ){

		// 得到一段 
		Paragraph paragraph; 
		paragraph = m_paragraphs.Item( (long)lc );

		// 得到一段的文本 
		Range range = paragraph.GetRange(); 
		CString strRangeText = range.GetText();
		range.ReleaseDispatch();

		//释放对象 
		paragraph.ReleaseDispatch();
		CString strTemp = strRangeText;
		strTemp.Trim();
		if(!strTemp.IsEmpty())
		{
			lstStr.push_back(strRangeText);
		}
	}
	std::list<CVocabularyQuestion*> lstQuestion;
	CListAutoClean<CVocabularyQuestion> clr(lstQuestion);
	CVocabularyQuestion* pVocabularyQuestion = NULL;
	for(std::list<CString>::const_iterator itr = lstStr.begin();
		itr != lstStr.end();++itr)
	{
		CString strText = (*itr);
		//先判断当前是否是一个新题目的开始
		BOOL bIsNewQuestion = FALSE;
		CString strCaption;
		hr = IsNewQuestionCaption(strText,bIsNewQuestion,strCaption);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bIsNewQuestion)
		{
			pVocabularyQuestion = new CVocabularyQuestion();
			pVocabularyQuestion->m_strCaption = strCaption;
			lstQuestion.push_back(pVocabularyQuestion);
			continue;
		}
		if(pVocabularyQuestion == NULL)
		{
			continue;
		}
		ASSERT(pVocabularyQuestion);
		BOOL bIsOption = FALSE;
		hr = IsQuestionOption(strText,bIsOption,pVocabularyQuestion->m_lstOption);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bIsOption)
		{
			continue;
		}
		BOOL bIsAnswer = FALSE;
		hr = IsQuestionAnswer(strText,bIsAnswer,pVocabularyQuestion->m_strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bIsAnswer)
		{
			continue;
		}
		//判断是否为指标
		BOOL bIsFactor = FALSE;
		hr = IsQuestionFactor(strText,bIsFactor,pVocabularyQuestion->m_lstFactor);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bIsFactor)
		{
			continue;
		}
	}
	CDlgVocabularyInputPreview dlg;
	dlg.m_plstVocabularyQuestion = &lstQuestion;
	dlg.m_pVault = m_pVault;
	dlg.m_pType = m_pType;
	dlg.DoModal();
	return S_OK;
}

HRESULT CVocabularyInputOutputQuestionHelper::ExeOutputFile(CString _strFile,std::list<CYDObjectRef*> *_plstObj)
{
	HRESULT hr = E_FAIL;
	hr = CreateBlankWord();
	if(FAILED(hr))
	{
		return hr;
	}
//	Paragraph   paragraph;//用来表示文档中某一段
	//paragraph=m_paragraphs.GetFirst();//得到第一段 
	//Range   rg=paragraph.GetRange(); 
	//输出格式如下
	/*41. He asked us to _____ them in carrying through their plan. 
		[A] provide	[C] assist 
		[B] arouse	[D] persist 
		答案：A   
		[1]进度	11
		[2]难度	3
		
	*/
	ASSERT(m_pVault);
	ASSERT(m_pType);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstFactorInfo;
	CListAutoClean<CYDObjectRef> clr(lstFactorInfo);
	CFactorInfoHelper helper;
	hr = helper.GetFactorInfoByVaultQType(pDB,(CYDObjectRef*)m_pVault,m_pType,&lstFactorInfo);
	if(FAILED(hr))
	{
		return hr;
	}
	Selection oSel = m_oWordApp.GetSelection();
	int index = 1;
	for(auto itr = _plstObj->begin();itr != _plstObj->end();++itr)
	{
		CString strChoiceTitle;
		hr = (*itr)->GetPropVal(FIELD_CHOICEQUESTION_TITLE,strChoiceTitle);
		CString strIndexTitle;
		strIndexTitle.Format(_T("%d.%s"),index,strChoiceTitle);
		oSel.TypeText(strIndexTitle);
		oSel.TypeParagraph();
		std::list<CString> lstChoices;
		CYDChoiceQuestionRef* pRef = (CYDChoiceQuestionRef*)(*itr);
		hr = pRef->GetOptionList(&lstChoices);
		if (FAILED(hr))
		{
			return hr;
		}
		//选择项
		TCHAR chXuhao = _T('A');
		for(auto itrChoice = lstChoices.begin();itrChoice != lstChoices.end();++itrChoice)
		{
			CString strChoice;
			strChoice.Format(_T("[%c]%s"),chXuhao++,(*itrChoice));
			oSel.TypeText(strChoice);
			oSel.TypeParagraph();
		}
		//答案
		CString strAnswer;
		hr = pRef->GetPropVal(FIELD_CHOICEQUESTION_ANSWER,strAnswer);
		oSel.TypeText(_T("答案：")+strAnswer);
		oSel.TypeParagraph();

		//指标
		int indexFactor = 1;
		for(auto itrFactorInfo = lstFactorInfo.begin();
			itrFactorInfo != lstFactorInfo.end();++itrFactorInfo)
		{
			CString strFactorName;
			hr = (*itrFactorInfo)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strFactorName);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strFieldName;
			hr = (*itrFactorInfo)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strFieldVal;
			hr = pRef->GetPropVal((CComBSTR)strFieldName,strFieldVal);
			if(FAILED(hr))
			{
				return hr;
			}
			if(!strFieldVal.IsEmpty())
			{
				//插入一个指标
				CString strFactor;
				strFactor.Format(_T("[%d]%s\t%s"),indexFactor++,strFactorName,strFieldVal);
				oSel.TypeText(strFactor);
				oSel.TypeParagraph();
			}
		}
		oSel.TypeParagraph();
		index++;
	}

	hr = SaveWord(_strFile);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CVocabularyInputOutputQuestionHelper::IsNewQuestionCaption(CString _strText,BOOL &_IsCaption,CString &_strCaption)
{
	HRESULT hr = E_FAIL;
	_IsCaption = FALSE;
	CString strLeft;//根据左边是数字+.的话，表示是一个题目的开始
	for(int i = 0;i < 6;i++)
	{
		if(_strText.GetAt(i) == _T('.'))
		{
			if(CDataHandler::StringIsDigit(strLeft))
			{
				//代表是一个新的题目
				_IsCaption = TRUE;
				_strCaption = _strText.Right(_strText.GetLength() - i-1);
				CDataHandler::TrimString(_strCaption);
				return S_OK;
			}
		}
		strLeft += _strText.GetAt(i);
	}
	return S_OK;
}

HRESULT CVocabularyInputOutputQuestionHelper::IsQuestionOption(CString _strText,BOOL &_IsOption,std::list<std::pair<CString,CString> > &_lstOption)
{
	HRESULT hr = E_FAIL;
	_strText.TrimLeft();
	_IsOption = FALSE;
	//判断_strText左边3位为：[A][B][C][D][E][F]
	CString strLeft = _strText.Left(3);
	for(int i = 0; i < 6;i++)
	{
		CString strTemp;
		strTemp.Format(_T("[%C]"),_T('A')+i);
		if(strLeft.CompareNoCase(strTemp) == 0)
		{
			//代表是选项
			_IsOption = TRUE;
			CStringArray arr;
			CDataHandler::SplitString(_strText, L'\t', arr);
			for(int j = 0; j < arr.GetCount();j++)
			{
				//strOption = "[A] dog",要解析
				CString strOption = arr.GetAt(j);
				CString strOptionName;//A B C D等
				strOptionName.Format(_T("%c"),strOption.GetAt(1));
				CDataHandler::TrimString(strOptionName);
				CString strOptionVal = strOption.Right(strOption.GetLength() - 4);//选项值
				CDataHandler::TrimString(strOptionVal);
				//要按照选项名进行排序
				BOOL bInsert = FALSE;
				for(std::list<std::pair<CString,CString> >::iterator itr = _lstOption.begin();
					itr != _lstOption.end();++itr)
				{
					CString strItrOptionName = (*itr).first;
					if(strItrOptionName.CompareNoCase(strOptionName) > 0)
					{
						//要在Itr之前插入一条记录
						_lstOption.insert(itr,std::make_pair(strOptionName,strOptionVal));
						bInsert = TRUE;
					}
				}
				if(!bInsert)
				{
					//说明在list前面还没有插入记录，要在最后插入一条记录
					_lstOption.push_back(std::make_pair(strOptionName,strOptionVal));
				}
				
			}
		}
	}
	return S_OK;
}

HRESULT CVocabularyInputOutputQuestionHelper::IsQuestionAnswer(CString _strText,BOOL &_IsAnswer,CString &_strAnswer)
{
	HRESULT hr = E_FAIL;
	_strText.TrimLeft();
	_IsAnswer = FALSE;
	//判断_strText左边3位为：答案：答案：
	CString strLeft = _strText.Left(3);
	if(strLeft.CompareNoCase(_T("答案:")) == 0||
		strLeft.CompareNoCase(_T("答案：")) == 0)
	{
		//代表是答案
		_IsAnswer = TRUE;
		_strAnswer = _strText.Right(_strText.GetLength() - 3);
		CDataHandler::TrimString(_strAnswer);
	}
	
	return S_OK;
}

HRESULT CVocabularyInputOutputQuestionHelper::IsQuestionFactor(CString _strText,BOOL &_IsFactor,std::list<std::pair<CString,CString> > &_lstFactor)
{
	//指标的格式如下
	//[1]题目难度	15
	//[1]代表序号
	//题目难度代表指标名
	//15代表指标值
	HRESULT hr = E_FAIL;
	_IsFactor = FALSE;
	CStringArray arr;
	CDataHandler::SplitString(_strText, L'\t', arr);
	if(arr.GetSize() != 2)
	{
		return S_FALSE;
	}
	CString strFirst = arr.GetAt(0);
	CDataHandler::TrimString(strFirst);
	//要移除序号后判断是否为指标名
	strFirst = strFirst.Right(strFirst.GetLength() -3);
	//判断是否为当前题型的指标名
	VARIANT_BOOL bIsFactorName = VARIANT_FALSE;
	hr = IsFactorName(strFirst,&bIsFactorName);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bIsFactorName)
	{
		//当前不是指标名
		return S_FALSE;
	}
	_IsFactor = TRUE;
	CString strFactorVal = arr.GetAt(1);
	CDataHandler::TrimString(strFactorVal);
	_lstFactor.push_back(std::make_pair(strFirst,strFactorVal));
	return S_OK;
}

