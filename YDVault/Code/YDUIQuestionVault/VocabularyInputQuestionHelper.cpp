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
		// �õ�һ�� 
		Paragraph paragraph; 
		paragraph = _paragraphs.Item( _index );

		// �õ�һ�ε��ı� 
		Range range = paragraph.GetRange(); 
		CString strRangeText = range.GetText();
		range.ReleaseDispatch();
		lstStr.push_back(strRangeText);

		//�ͷŶ��� 
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
	// �����ж��е�ÿһ�ζ����������� 
	for(long lc = 1 ; lc < paCounst+1 ; lc++ ){

		// �õ�һ�� 
		Paragraph paragraph; 
		paragraph = m_paragraphs.Item( (long)lc );

		// �õ�һ�ε��ı� 
		Range range = paragraph.GetRange(); 
		CString strRangeText = range.GetText();
		range.ReleaseDispatch();

		//�ͷŶ��� 
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
		//���жϵ�ǰ�Ƿ���һ������Ŀ�Ŀ�ʼ
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
		//�ж��Ƿ�Ϊָ��
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
	CString strLeft;//�������������+.�Ļ�����ʾ��һ����Ŀ�Ŀ�ʼ
	for(int i = 0;i < 6;i++)
	{
		if(_strText.GetAt(i) == _T('.'))
		{
			if(CDataHandler::StringIsDigit(strLeft))
			{
				//������һ���µ���Ŀ
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
	//�ж�_strText���3λΪ��[A][B][C][D][E][F]
	CString strLeft = _strText.Left(3);
	for(int i = 0; i < 6;i++)
	{
		CString strTemp;
		strTemp.Format(_T("[%C]"),_T('A')+i);
		if(strLeft.CompareNoCase(strTemp) == 0)
		{
			//������ѡ��
			_IsOption = TRUE;
			CStringArray arr;
			CDataHandler::SplitString(_strText, L'\t', arr);
			for(int j = 0; j < arr.GetCount();j++)
			{
				//strOption = "[A] dog",Ҫ����
				CString strOption = arr.GetAt(j);
				CString strOptionName;//A B C D��
				strOptionName.Format(_T("%c"),strOption.GetAt(1));
				CString strOptionVal = strOption.Right(strOption.GetLength() - 4);//ѡ��ֵ
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
	//�ж�_strText���3λΪ���𰸣��𰸣�
	CString strLeft = _strText.Left(3);
	if(strLeft.CompareNoCase(_T("��:")) == 0||
		strLeft.CompareNoCase(_T("�𰸣�")) == 0)
	{
		//�����Ǵ�
		_IsAnswer = TRUE;
		_strAnswer = _strText.Right(_strText.GetLength() - 3);
	}
	
	return S_OK;
}

HRESULT CVocabularyInputQuestionHelper::IsQuestionFactor(CString _strText,BOOL &_IsFactor,std::list<std::pair<CString,CString> > &_lstFactor)
{
	//ָ��ĸ�ʽ����
	//[1]��Ŀ�Ѷ�	15
	//[1]�������
	//��Ŀ�Ѷȴ���ָ����
	//15����ָ��ֵ
	HRESULT hr = E_FAIL;
	_IsFactor = FALSE;
	CStringArray arr;
	CDataHandler::SplitString(_strText, L'\t', arr);
	if(arr.GetSize() != 2)
	{
		return S_FALSE;
	}
	CString strFirst = arr.GetAt(0);
	//Ҫ�Ƴ���ź��ж��Ƿ�Ϊָ����
	strFirst = strFirst.Right(strFirst.GetLength() -3);
	//�ж��Ƿ�Ϊ��ǰ���͵�ָ����
	VARIANT_BOOL bIsFactorName = VARIANT_FALSE;
	hr = IsFactorName(strFirst,&bIsFactorName);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bIsFactorName)
	{
		//��ǰ����ָ����
		return S_FALSE;
	}
	_IsFactor = TRUE;
	CString strFactorVal = arr.GetAt(1);
	_lstFactor.push_back(std::make_pair(strFirst,strFactorVal));
	return S_OK;
}

