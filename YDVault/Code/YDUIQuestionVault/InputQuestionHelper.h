#pragma once
#include "../wordbase/msword.h"
class CInputQuestionHelper
{
public:
	CInputQuestionHelper(QTYPE qType);
	~CInputQuestionHelper(void);
public:
	virtual HRESULT ExeInputFile(CString _strFile) = 0;
protected:
	QTYPE	m_qType;
	_Application m_oWordApp ; 
	Selection m_oSel ; 
	Documents m_oDocs ; 
	_Document m_oDoc ; 
	Paragraphs m_paragraphs ;
	BOOL	m_bCreateWordSuc;
	HRESULT CreateWord(CString _strFile);
};

