#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"

#include "VocabularyInputQuestionHelper.h"

class CBCGPKeyGridCtrl;
class CFactorInfoHelper;
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
	CYDObjectRef*		m_pVault;
	CYDObjectRef*		m_pType;
private:
	CBCGPKeyGridCtrl						m_Grid;
	HRESULT InsertRowByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion);
	//根据一条根记录插入一个选择题
	HRESULT InsertQuestionByRow(CBCGPGridRow* _pRootRow);
	//得到_pRow的选项名和选项值
	void    GetOption(CBCGPGridRow* _pRow,CString &_strOptionName,CString &_strOption);
	void    GetFactor(CBCGPGridRow* _pRow,CString &_strFactorName,CString &_strFactor);
	HRESULT SetFactorProp(std::list<CYDObjectRef*> *_plstFactorInfo,
							CString _strFactorName,CString _strFactor,
							CYDObjectRef* _pQRef,CFactorInfoHelper* _phelper);
public:
	afx_msg void OnBnClickedOk();
};
