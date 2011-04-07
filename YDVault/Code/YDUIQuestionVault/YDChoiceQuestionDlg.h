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
	HRESULT CreateChoice(VARIANT* _valChoice);//有列表结合生成选项

	HRESULT Fill();
	HRESULT EnableQuestionArea(BOOL bReadonly);
	HRESULT UpdateQuestionArea();

	HRESULT GetOption(std::list<CString>* pListOption);
	HRESULT FillOption(std::list<CString>* pListOption);

	BOOL VocabularyValid();
	BOOL ValidOther();
	HRESULT InitKnowledgeList();//初始化知识点
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
	CYDQuestionRef* m_pParentQuestionRef;//当前选择题如果是挂在题库下，则m_pParentQuestionRef = NULL
										 //当前选择题如果挂在其他题目下，如阅读题等
										//等于空表示当前的阅读题是新增的
										//不为空表示阅读题在数据库中已经存在
	CString m_strAnswerDesc;
	virtual INT_PTR DoModal();
};
