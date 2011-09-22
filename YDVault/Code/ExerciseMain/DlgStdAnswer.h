#pragma once
#include "SelectorEngine.h"
#include "wmpplayer4.h"
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "StdAnswerDetialDlg.h"
// CDlgStdAnswer dialog

class CStdAnswerBCGPGridCtrl :
	public CBCGPKeyGridCtrl
{
public:
	DECLARE_DYNAMIC(CStdAnswerBCGPGridCtrl)
	CStdAnswerBCGPGridCtrl(void);
	~CStdAnswerBCGPGridCtrl(void);
protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

};





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
	CStdAnswerBCGPGridCtrl	m_listGrid;
	CWMPPlayer4	    m_WMPlay;
	HRESULT InsertList(CYDQuestionRef* _pQuestionRef,
					  CBCGPGridRow* _pParentRow,
					  CBCGPGridRow*& _pInsertRow);
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedOk();
};
