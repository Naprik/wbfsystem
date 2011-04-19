#include "StdAfx.h"
#include "InputQuestionHelper.h"


CInputQuestionHelper::CInputQuestionHelper(QTYPE qType)
{
	m_qType = qType;
	m_bCreateWordSuc = FALSE;
}


CInputQuestionHelper::~CInputQuestionHelper(void)
{
	if(m_bCreateWordSuc)
	{
		//�ͷ�
		m_paragraphs.ReleaseDispatch();
		m_oDoc.ReleaseDispatch();
		m_oDocs.ReleaseDispatch();
		m_oSel.ReleaseDispatch();
		m_oWordApp.ReleaseDispatch();
	}
}

HRESULT CInputQuestionHelper::CreateWord(CString _strFile)
{
	HRESULT hr = E_FAIL;
	ASSERT(!m_bCreateWordSuc);
	COleVariant vTrue((short)TRUE), 
                  vFalse((short)FALSE), 
                  vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	
	CStringArray aryText;

		// ���Ƚ���һ��wordʵ������ 
	if (!m_oWordApp.CreateDispatch(TEXT("Word.Application")) ){
		DWORD dwErr = GetLastError();
		AfxMessageBox(_T("�밲װWord����"));
		return E_FAIL; // һ��Ҫ���أ����������� 
	}

	// ��ʾ�˶����ļ� ����Ҳ���Բ����ô˾䣬����ʾ�����ļ� 
	m_oWordApp.SetVisible(FALSE);

	// �����ĵ� 
	m_oDocs = m_oWordApp.GetDocuments();

	// ��һ�����ĵ�����e:\\BuildJobEdit.rtf�ļ���������ӵ����ĵ��� 
	CComVariant tpl(_strFile), NewTemplate(false), DocType(0), Visble; 
	m_oDoc = m_oDocs.Add(&tpl,&NewTemplate,&DocType,&Visble); 
      
	// ѡ���ĵ��е��������� 
	m_oSel = m_oWordApp.GetSelection(); 
	m_oSel.WholeStory();// �˾�����У��������������Ч��

	// �õ����ж� 
	m_paragraphs = m_oSel.GetParagraphs(); 


	m_bCreateWordSuc = TRUE;
	return S_OK;
}