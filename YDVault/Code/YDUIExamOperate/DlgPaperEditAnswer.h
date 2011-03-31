#pragma once
#include "Resource.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

class CAnswerRecord;

// CDlgPaperEditAnswer dialog

class CDlgPaperEditAnswer : public CDialog
{
	DECLARE_DYNAMIC(CDlgPaperEditAnswer)

public:
	CDlgPaperEditAnswer(std::list<CAnswerRecord*>* _pListRecord,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPaperEditAnswer();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_PAPER_EDIT_ANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPKeyGridCtrl	m_lstGridAnswer;
	std::list<CAnswerRecord*>* m_pListRecord;
	HRESULT InsertItem(CAnswerRecord* _pRecord);
	HRESULT ValidateData(VARIANT_BOOL* _bValidate);
public:
	afx_msg void OnBnClickedOk();
};
