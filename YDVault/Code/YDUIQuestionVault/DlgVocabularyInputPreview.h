#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"

#include "VocabularyInputQuestionHelper.h"

class CBCGPKeyGridCtrl;
// CDlgVocabularyInputPreview dialog

class CDlgVocabularyInputPreview : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVocabularyInputPreview)

public:
	CDlgVocabularyInputPreview(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgVocabularyInputPreview();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_VOCABULARY_INPUT_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::list<CVocabularyQuestion*>*	m_plstVocabularyQuestion;
	virtual BOOL OnInitDialog();
private:
	CBCGPKeyGridCtrl						m_Grid;
	HRESULT InsertRowByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion);
};
