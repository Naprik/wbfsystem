#include "StdAfx.h"
#include "VocabularyInputQuestionHelper.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "DlgVocabularyInputPreview.h"

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


CVocabularyInputQuestionHelper::CVocabularyInputQuestionHelper(OBJID _IDVault)
	:CInputQuestionHelper(_IDVault,6)
{
}


CVocabularyInputQuestionHelper::~CVocabularyInputQuestionHelper(void)
{
}

HRESULT CVocabularyInputQuestionHelper::ExeInputFile(CString _strFile)
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
	dlg.DoModal();
	return S_OK;
}

HRESULT CVocabularyInputQuestionHelper::IsNewQuestionCaption(CString _strText,BOOL &_IsCaption,CString &_strCaption)
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
				return S_OK;
			}
		}
		strLeft += _strText.GetAt(i);
	}
	return S_OK;
}

HRESULT CVocabularyInputQuestionHelper::IsQuestionOption(CString _strText,BOOL &_IsOption,std::list<std::pair<CString,CString> > &_lstOption)
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
				CString strOptionVal = strOption.Right(strOption.GetLength() - 4);//选项值
				_lstOption.push_back(std::make_pair(strOptionName,strOptionVal));
			}
		}
	}
	return S_OK;
}

HRESULT CVocabularyInputQuestionHelper::IsQuestionAnswer(CString _strText,BOOL &_IsAnswer,CString &_strAnswer)
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
	}
	
	return S_OK;
}

HRESULT CVocabularyInputQuestionHelper::IsQuestionFactor(CString _strText,BOOL &_IsFactor,std::list<std::pair<CString,CString> > &_lstFactor)
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
	_lstFactor.push_back(std::make_pair(strFirst,strFactorVal));
	return S_OK;
}

