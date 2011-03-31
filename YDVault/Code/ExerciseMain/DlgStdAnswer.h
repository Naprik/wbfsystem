#pragma once
#include "SelectorEngine.h"
#include "wmpplayer4.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

// CDlgStdAnswer dialog

class CDlgStdAnswer : public CDialog
{
	DECLARE_DYNAMIC(CDlgStdAnswer)

public:
	CDlgStdAnswer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStdAnswer();

// Dialog Data
	enum { IDD = IDD_DLG_EM_STDANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CLISTQUESTION*	m_pListQuestion;
private:
	CBCGPKeyGridCtrl	m_listGrid;
	CWMPPlayer4	    m_WMPlay;
	HRESULT InsertList(CYDQuestionRef* _pQuestionRef,
					  CBCGPGridRow* _pParentRow,
					  CBCGPGridRow*& _pInsertRow);
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedOk();
};
