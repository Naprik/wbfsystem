#pragma once
#include "../YDFormUIBase/YDQuestionDlg.h"
#include "resource.h"
#include "afxcmn.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

// CYDTranslationQuestionDlg dialog
class CYDChoiceQuestionRef;
class CYDLinkRef;
class CYDTranslationQuestionDlg : public CYDQuestionDlg
{
	DECLARE_DYNAMIC(CYDTranslationQuestionDlg)

public:
	CYDTranslationQuestionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYDTranslationQuestionDlg();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_TRANSLATION_QUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCqAdd();
	afx_msg void OnBnClickedButtonCqModify();
	afx_msg void OnBnClickedButtonAddkp();
	afx_msg void OnBnClickedButtonRemovekp();

private:
	BOOL ValidateData(OPERATION op);
	HRESULT CreateQuestionRef(CYDChoiceQuestionRef* &_pRef);
	HRESULT CreateLinkRef(CYDLinkRef* &_pLinkRef);
	HRESULT UpdateQuestionArea();
	HRESULT EnableQuestionArea(BOOL bEnable);
	HRESULT UpdateQuestionRef(CYDChoiceQuestionRef* _pRef);
	BOOL IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper);
	HRESULT PersistKnowledge(CYDObjectRef* _pParentRef);//保存或更新知识点
	HRESULT InitKnowledge();//初始化知识点

private:
	std::list<CYdObjWrapper*>	m_lstAddKPs;
	std::list<CYdObjWrapper*>	m_lstDeleteKPs;
	CBCGPKeyGridCtrl			m_GridIndicator;

public:
	int m_iHardLevel;
	CString m_strCreateDate;
	CString m_strCreator;
	CListCtrl m_listCtrlKpAll;
	CListCtrl m_listCtrlKpRelated;
	CString m_strArticle;
	CString m_strAnswer;
	virtual INT_PTR DoModal();
};
