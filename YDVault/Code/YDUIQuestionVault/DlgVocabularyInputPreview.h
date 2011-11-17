#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"

#include "VocabularyInputOutputQuestionHelper.h"

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
	HRESULT InsertRowByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion,int _index);
	
	HRESULT UpdateQuestionByRow(CBCGPGridRow* _pRootRow);
	//����һ������¼����һ��ѡ����
	HRESULT InsertQuertionByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion);
	//�õ�_pRow��ѡ������ѡ��ֵ
	void    GetOption(CBCGPGridRow* _pRow,CString &_strOptionName,CString &_strOption);
	void    GetFactor(CBCGPGridRow* _pRow,CString &_strFactorName,CString &_strFactor);
	HRESULT SetFactorProp(std::list<CYDObjectRef*> *_plstFactorInfo,
							CString _strFactorName,CString _strFactor,
							CYDObjectRef* _pQRef,CFactorInfoHelper* _phelper);
	std::list<CVocabularyQuestion*>::const_iterator	m_ItrCur;
	int											    m_iIndex;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonMore();
	CString m_strQNum;
};
