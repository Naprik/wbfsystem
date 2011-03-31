#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "../ydformUIBASE/YDQuestionDlg.h"

class CGridListCtrlQuestion :
	public CBCGPKeyGridCtrl
{
public:
	DECLARE_DYNAMIC(CGridListCtrlQuestion)
	CGridListCtrlQuestion(void);
	~CGridListCtrlQuestion(void);
protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	HRESULT CreateQuestionDlg(QTYPE qType, CYDQuestionDlg* &_pDlg);

public:
	OBJID	m_VaultID;
};
