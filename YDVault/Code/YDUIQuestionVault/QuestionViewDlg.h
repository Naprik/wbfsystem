#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "wmpplayer4.h"

class CYDQuestionRef;
// CQuestionViewDlg dialog

class CQuestionViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CQuestionViewDlg)

public:
	CQuestionViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQuestionViewDlg();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_QVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnPlayMedia();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strQuestion;
	CYDQuestionRef* m_pQuestionRef;
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
private:
	std::list<CYDMediaRef*> m_lstMedia;//��ǰ�����Ŀ����������Ӱ��ͼ��ȶ���
	CBCGPKeyGridCtrl						m_Grid;
	CWMPPlayer4	    m_WMPlay;
};
