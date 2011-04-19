#include "StdAfx.h"
#include "VocabularyInputQuestionHelper.h"


CVocabularyInputQuestionHelper::CVocabularyInputQuestionHelper(void)
	:CInputQuestionHelper(QTYPE_VOCABULARY)
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

	}
	return S_OK;
}