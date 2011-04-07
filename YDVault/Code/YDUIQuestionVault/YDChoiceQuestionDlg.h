#pragma once
#include "../YDFormUIBase/YDQuestionDlg.h"
#include <list>
#include "afxcmn.h"
#include "Resource.h"
#include "../UIBase/BCGPKeyGridCtrl.h"


// CYDChoiceQuestionDlg dialog
class CYDChoiceQuestionRef;
class CYDLinkRef;
class CYDQuestionRef;

class CYDChoiceQuestionDlg : public CYDQuestionDlg
{
	DECLARE_DYNAMIC(CYDChoiceQuestionDlg)

public:
	CYDChoiceQuestionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYDChoiceQuestionDlg();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_CHOICE_QUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCqModify();

	afx_msg void OnBnClickedButtonCqAdd();
	afx_msg void OnBnClickedButtonCqClose();

	afx_msg void OnBnClickedButtonCqAddkp();
	afx_msg void OnBnClickedButtonCqRemovekp();
private:
	HRESULT CreateQuestionRef(CYDChoiceQuestionRef* &_pRef);
	HRESULT CreateLinkRef(CYDLinkRef* &_pLinkRef);
	HRESULT UpdateQuestionRef(CYDChoiceQuestionRef* _pRef);
	HRESULT CreateChoice(VARIANT* _valChoice);//���б�������ѡ��

	HRESULT Fill();
	HRESULT EnableQuestionArea(BOOL bReadonly);
	HRESULT UpdateQuestionArea();

	HRESULT GetOption(std::list<CString>* pListOption);
	HRESULT FillOption(std::list<CString>* pListOption);

	BOOL VocabularyValid();
	BOOL ValidOther();
	HRESULT InitKnowledgeList();//��ʼ��֪ʶ��
	BOOL IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper);

	BOOL EnableCtrl(QTYPE type);
	BOOL ValidateOption();
	BOOL ValidateAsnwer();

private:
	CString m_strCode;
	CString m_strTitle;
	CString m_strAnswer;
	int		m_iHardLevel;
	CString m_strCreator;
	CString m_strCreateDate;
	//CListCtrl m_lstChoiceQuestion;
	
	CYDChoiceQuestionRef* m_pCQ;
	CBCGPKeyGridCtrl	m_gridOption;
	
private:
	CListCtrl m_listCtrlKpAll;
	CListCtrl m_listCtrlKpRelated;

	std::list<CYdObjWrapper*>	m_lstAddKPs;
	std::list<CYdObjWrapper*>	m_lstDeleteKPs;
	CBCGPKeyGridCtrl			m_GridIndicator;
public:
	CYDQuestionRef* m_pParentQuestionRef;//��ǰѡ��������ǹ�������£���m_pParentQuestionRef = NULL
										 //��ǰѡ�����������������Ŀ�£����Ķ����
										//���ڿձ�ʾ��ǰ���Ķ�����������
										//��Ϊ�ձ�ʾ�Ķ��������ݿ����Ѿ�����
	CString m_strAnswerDesc;
	virtual INT_PTR DoModal();
};
