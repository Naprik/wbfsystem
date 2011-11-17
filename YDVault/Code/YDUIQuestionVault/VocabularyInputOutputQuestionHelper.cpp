#include "StdAfx.h"
#include "VocabularyInputOutputQuestionHelper.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "DlgVocabularyInputPreview.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef\YDChoiceQuestionRef.h"
#include "../ObjHelper\FactorInfoHelper.h"
#include "../Base\StdioFileEx.h"
#include <vector>
#include "../UIBase\PdemWait.h"

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

HRESULT CVocabularyInputOutputQuestionHelper::ExeInputFileFromTxt(CString _strFile) 
{
	HRESULT hr = E_FAIL;
	std::vector<std::pair<CString, CString>> pairSub;//ǰ������Ŀ�������Ǵ�
	try
	{
		CStdioFileEx file(_strFile,CFile::modeRead);
		BOOL bHas = FALSE;
		do 
		{
			CString strTxt;
			bHas = file.ReadString(strTxt);
			strTxt.TrimLeft();
			strTxt.TrimRight();
			if(strTxt.IsEmpty() || strTxt.GetLength() <= 1)
			{
				continue;
			}
			CStringArray arrStr;
			CDataHandler::SplitString(strTxt,_T(' '),arrStr);
			if(arrStr.GetSize() <=1)
			{
				continue;
			}
			//arrStr��ǰ��Ŀ϶���Ӣ�ģ������������
			//��û����������ʱ����֮ǰ�����д浽strEng�����İ���֮��Ĵ浽strChs��
			CString strEng,strChs;//��Ŀ��Ӣ�ģ���������
			for(int i =0; i < arrStr.GetCount();i++)
			{
				CString str = arrStr.GetAt(i);
				BOOL bIsChs = HasChs(str);
				if(bIsChs)
				{
					strChs = str;
					i++;
					while(i < arrStr.GetCount())
					{
						strChs += _T(' ');
						strChs += arrStr.GetAt(i);
						i++;
					}
					break;
				}
				else
				{
					if(i != 0)
					{
						strEng += _T(" ");
					}
					strEng += str;
				}
			}
			if(strEng.IsEmpty() || strChs.IsEmpty())
			{
				continue;
			}
			pairSub.push_back(std::make_pair(strEng, strChs));
		} while (bHas);

		file.Close();

	}
	catch (CFileException* /*e*/)
	{
		CString strMsg;
		strMsg.Format(_T("��ȡ�ļ�%s��������"),_strFile);
		AfxMessageBox(strMsg);

		return S_FALSE;
	}
	catch (...)
	{
		CString strMsg;
		strMsg.Format(_T("��ȡ�ļ�%s��������"),_strFile);
		AfxMessageBox(strMsg);

		return S_FALSE;
	}
	if(pairSub.size() == 0)
	{
		AfxMessageBox(_T("û�ж�ȡ�����ݣ�"));
		return S_FALSE;
	}
	srand(GetTickCount());
	int iSize = pairSub.size();
	std::list<CVocabularyQuestion*> lstQuestion;
	CListAutoClean<CVocabularyQuestion> clr(lstQuestion);
	CVocabularyQuestion* pVocabularyQuestion = NULL;
	int itrIndex = 0;
	for(auto itr = pairSub.begin();itr != pairSub.end();++itr,++itrIndex)
	{
		pVocabularyQuestion = new CVocabularyQuestion();
		lstQuestion.push_back(pVocabularyQuestion);
		pVocabularyQuestion->m_strCaption = (*itr).first;
		//���ĸ�ѡ�����һ����(*itr).first�����������Ǵ�pairSub�����ȡ
		
		UINT iRandOption = RandOption();
		ASSERT(iRandOption < 4);
		std::list<UINT> lstOtherOption;
		RandOtherOption(iSize,itrIndex,lstOtherOption);
		ASSERT(lstOtherOption.size() == 3);
		std::list<UINT>::const_iterator itrOtherOpton = lstOtherOption.begin();
		for(UINT i = 0; i < 4;i++)
		{
			CString strOptionFirst,strOptionSecond;
			strOptionFirst.Format(_T("%c"),_T('A')+i);
			if(i == iRandOption)
			{
				strOptionSecond = (*itr).second;
				pVocabularyQuestion->m_strAnswer = strOptionFirst;
			}
			else
			{
				UINT index = *itrOtherOpton;
				itrOtherOpton++;
				strOptionSecond = pairSub[index].second;
			}
			pVocabularyQuestion->m_lstOption.push_back(std::make_pair(strOptionFirst,strOptionSecond));
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
	//Selection oSel = m_oWordApp.GetSelection();
	int index = 1;
	CPdemWait	wait(_T("���Ժ����ڵ�������..."),FALSE,_plstObj->size());
	wait.BeginWait();
	for(auto itr = _plstObj->begin();itr != _plstObj->end();++itr)
	{
		Paragraph paragraph = m_paragraphs.GetLast();
		Range rg = paragraph.GetRange();
		CString strText = rg.GetText();
		CString strChoiceTitle;
		hr = (*itr)->GetPropVal(FIELD_CHOICEQUESTION_TITLE,strChoiceTitle);
		CString strIndexTitle;
		strIndexTitle.Format(_T("%d.%s\r\n"),index,strChoiceTitle);
		strText += strIndexTitle;
	//	oSel.TypeText(strIndexTitle);
	//	oSel.TypeParagraph();
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
			strChoice.Format(_T("[%c]%s\r\n"),chXuhao++,(*itrChoice));
			strText += strChoice;
			//oSel.TypeText(strChoice);
			//oSel.TypeParagraph();
		}
		//��
		CString strAnswer;
		hr = pRef->GetPropVal(FIELD_CHOICEQUESTION_ANSWER,strAnswer);
		CString strAnswerFormat;
		strAnswerFormat.Format(_T("�𰸣�%s\r\n"),strAnswer);
		strText += strAnswerFormat;

		//oSel.TypeText(_T("�𰸣�")+strAnswer);
		//oSel.TypeParagraph();

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
				strFactor.Format(_T("[%d]%s\t%s\r\n"),indexFactor++,strFactorName,strFieldVal);
				strText += strFactor;
				//oSel.TypeText(strFactor);
				//oSel.TypeParagraph();
			}
		}
		//oSel.TypeParagraph();
		strText += _T("\r\n");
		rg.SetText(strText);
		rg.ReleaseDispatch();
		paragraph.ReleaseDispatch();
	//	Sleep(10);
		index++;
		wait.StepIt();
	}

	hr = SaveWord(_strFile);
	if(FAILED(hr))
	{
		return hr;
	}
	wait.Close();
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

BOOL CVocabularyInputOutputQuestionHelper::HasChs(CString _str)//�ж�_str���Ƿ�������
{
	CString strEng = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890`=~!@#$%^&*()_+[]{}\\|/?.>,<;:'-?<>/\"");
	for(int i = 0; i < _str.GetLength();i++)
	{
		if(strEng.Find(_str.GetAt(i)) == -1)
		{
			return TRUE;
		}
	}
	return FALSE;
}

UINT	CVocabularyInputOutputQuestionHelper::RandOption()
{
	
	return rand() % 4;
}


//�������������뵱ǰ����ͬ�������໥֮��Ҳ������ͬ
BOOL CVocabularyInputOutputQuestionHelper::RandOtherOption(UINT iSize,UINT iCurItr,std::list<UINT> &_lstOther)
{
	UINT iIndex1 = rand() % iSize;
	while(iIndex1 == iCurItr)
	{
		iIndex1 = rand() % iSize;
	}
	int iIndex2 = rand() % iSize;
	while(iIndex2 == iCurItr || iIndex2 == iIndex1)
	{
		iIndex2 = rand() % iSize;
	}
	int iIndex3 = rand() % iSize;
	while(iIndex3 == iCurItr ||
		  iIndex3 == iIndex1 ||
		  iIndex3 == iIndex2 )
	{
		iIndex3 = rand() % iSize;
	}
	_lstOther.push_back(iIndex1);
	_lstOther.push_back(iIndex2);
	_lstOther.push_back(iIndex3);
	return TRUE;
}