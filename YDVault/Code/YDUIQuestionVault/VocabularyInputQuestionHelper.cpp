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

	}
	return S_OK;
}