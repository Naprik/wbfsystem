#pragma once
#include "../YDFormUIBase/YdFormView.h"
#include "GridListCtrlQuestion.h"
#include "afxwin.h"

class CYDQuestionVault;
class CBCGPKeyGridCtrl;
class CYDQuestionType;
class CYDPaper;
class CYDQuestionRef;

// CGeneratePaperFormView form view

class BASE_DLL_API CGeneratePaperFormView : public CYdFormView
{
	DECLARE_DYNCREATE(CGeneratePaperFormView)

protected:
	CGeneratePaperFormView();           // protected constructor used by dynamic creation
	virtual ~CGeneratePaperFormView();

public:
	enum { IDD = IDD_YDUIQUESTIONVAULT_FORM_GENERATEPAPER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

private:
	HRESULT FillGrid();
	HRESULT InsertItem(CYDQuestionType* pType, CBCGPGridRow* pParentRow);

	HRESULT FillPreview();

public:
	afx_msg void OnBnClickedBtnGpOk();
	afx_msg void OnBnClickedBtnGpCancel();
	afx_msg void OnBnClickedButtonResel();
	afx_msg void OnBnClickedButtonInput();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonObsolete();

private:
	CBCGPKeyGridCtrl		m_gridList;
	CGridListCtrlQuestion		m_gridPreview;
	CYDPaper*			m_pPaper;

	std::list<CYDQuestionType*>		m_lstClean;

private:
	HRESULT CreateQuestionDlg(CYDQuestionRef* _pQuestion,CYDQuestionDlg* &_pDlg);
	//得到当前试题类型所属的题库
	HRESULT GetQuestionVault(CYDQuestionVault* &_pQV);
	//根据题目得到题型
	HRESULT GetQuestionType(CYDQuestionRef* _pQuestion,CYDQuestionType* &_pType);
	//初始化试题库模版
	HRESULT InitQuestionTemplate(std::list<CString> &_lstTemplateName);
public:
	afx_msg void OnCbnSelchangeCmbPapertemp();
	CComboBox m_cmbPaperTemp;

private:
	CString		m_strPaperFolder;
	OBJID		m_VaultID;
};


