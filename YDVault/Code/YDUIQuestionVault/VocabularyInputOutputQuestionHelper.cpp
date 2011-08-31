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
//	Paragraph   paragraph;//������ʾ�ĵ���ĳһ��
	//paragraph=m_paragraphs.GetFirst();//�õ���һ�� 
	//Range   rg=paragraph.GetRange(); 
	//�����ʽ����
	/*41. He asked us to _____ them in carrying through their plan. 
		[A] provide	[C] assist 
		[B] arouse	[D] persist 
		�𰸣�A   
		[1]����	11
		[2]�Ѷ�	3
		
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
		//ѡ����
		TCHAR chXuhao = _T('A');
		for(auto itrChoice = lstChoices.begin();itrChoice != lstChoices.end();++itrChoice)
		{
			CString strChoice;
			strChoice.Format(_T("[%c]%s"),chXuhao++,(*itrChoice));
			oSel.TypeText(strChoice);
			oSel.TypeParagraph();
		}
		//��
		CString strAnswer;
		hr = pRef->GetPropVal(FIELD_CHOICEQUESTION_ANSWER,strAnswer);
		oSel.TypeText(_T("�𰸣�")+strAnswer);
		oSel.TypeParagraph();

		//ָ��
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
				//����һ��ָ��
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
				CDataHandler::TrimString(strOptionName);
				CString strOptionVal = strOption.Right(strOption.GetLength() - 4);//ѡ��ֵ
				CDataHandler::TrimString(strOptionVal);
				//Ҫ����ѡ������������
				BOOL bInsert = FALSE;
				for(std::list<std::pair<CString,CString> >::iterator itr = _lstOption.begin();
					itr != _lstOption.end();++itr)
				{
					CString strItrOptionName = (*itr).first;
					if(strItrOptionName.CompareNoCase(strOptionName) > 0)
					{
						//Ҫ��Itr֮ǰ����һ����¼
						_lstOption.insert(itr,std::make_pair(strOptionName,strOptionVal));
						bInsert = TRUE;
					}
				}
				if(!bInsert)
				{
					//˵����listǰ�滹û�в����¼��Ҫ��������һ����¼
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
	//�ж�_strText���3λΪ���𰸣��𰸣�
	CString strLeft = _strText.Left(3);
	if(strLeft.CompareNoCase(_T("��:")) == 0||
		strLeft.CompareNoCase(_T("�𰸣�")) == 0)
	{
		//�����Ǵ�
		_IsAnswer = TRUE;
		_strAnswer = _strText.Right(_strText.GetLength() - 3);
		CDataHandler::TrimString(_strAnswer);
	}
	
	return S_OK;
}

HRESULT CVocabularyInputOutputQuestionHelper::IsQuestionFactor(CString _strText,BOOL &_IsFactor,std::list<std::pair<CString,CString> > &_lstFactor)
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
	CDataHandler::TrimString(strFirst);
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
	CDataHandler::TrimString(strFactorVal);
	_lstFactor.push_back(std::make_pair(strFirst,strFactorVal));
	return S_OK;
}

