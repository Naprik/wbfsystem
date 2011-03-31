#pragma once
#include "../YDFormUIBase/YDQuestionDlg.h"

// CYDCompositionQuestionDlg dialog
class CYDArticleQuestionRef;
class CYDLinkRef;

class CYDCompositionQuestionDlg : public CYDQuestionDlg
{
	DECLARE_DYNAMIC(CYDCompositionQuestionDlg)

public:
	CYDCompositionQuestionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYDCompositionQuestionDlg();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_COMPOSITION_QUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strArticle;
	CString m_strAnswer;
	int m_iHardLevel;
	CString m_strCreateDate;
	CString m_strCreator;
	afx_msg void OnBnClickedButtonCqAdd();
	afx_msg void OnBnClickedButtonCqModify();

	afx_msg void OnBnClickedButtonAddkp();
	afx_msg void OnBnClickedButtonRemovekp();
	virtual BOOL OnInitDialog();
	BOOL IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper);
	HRESULT PersistKnowledge(CYDObjectRef* _pParentRef);//保存或更新知识点
	HRESULT InitKnowledge();//初始化知识点

private:
	std::list<CYdObjWrapper*>	m_lstAddKPs;
	std::list<CYdObjWrapper*>	m_lstDeleteKPs;

	CListCtrl m_listCtrlKpAll;
	CListCtrl m_listCtrlKpRelated;
private:
	BOOL ValidateData(OPERATION op);
	HRESULT CreateQuestionRef(CYDArticleQuestionRef* &_pRef);
	HRESULT CreateLinkRef(CYDLinkRef* &_pLinkRef);
	HRESULT UpdateQuestionArea();
	HRESULT EnableQuestionArea(BOOL bEnable);
	HRESULT UpdateQuestionRef(CYDArticleQuestionRef* _pRef);
public:
	afx_msg void OnBnClickedCancel();
	virtual INT_PTR DoModal();
};
