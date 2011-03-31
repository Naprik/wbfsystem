#pragma once

#include <list>

class CYDQuestionVault;
class CBCGPGridCtrl;
class CYDQuestionType;
class CYDPaper;
// CPaperMaker dialog

class CPaperMaker : public CDialog
{
	DECLARE_DYNAMIC(CPaperMaker)

public:
	CPaperMaker(CYDQuestionVault*& pVault, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaperMaker();

// Dialog Data
	enum { IDD = IDD_DIALOG_GENERATEPAPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:
	HRESULT FillGrid();
	HRESULT InsertItem(CYDQuestionType* pType);

private:
	CYDQuestionVault*	m_pVault;
	CBCGPGridCtrl		m_gridList;

	CYDPaper*			m_pPaper;

	std::list<CYDQuestionType*>		m_lstClean;
public:
	afx_msg void OnBnClickedBtnGpOk();
	afx_msg void OnBnClickedBtnGpCancel();
	afx_msg void OnBnClickedBtnGpSave();
};
