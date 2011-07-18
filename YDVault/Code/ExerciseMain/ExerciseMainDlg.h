#pragma once
#include "afxwin.h"
#include "BCGPButton.h"
#include "../ObjRef/YDQuestionRef.h"
#include "afxcmn.h"
#include "SelectorEngine.h"
#include "ArticleDlg.h"
#include "ListenDlg.h"
#include "DefaultDlg.h"
#include "WriteDlg.h"
#include "ChoiceDlg.h"
#include "ExerciseQuestionRecord.h"
#include "QuestionDlg.h"
#include "ArticleListenDlgh.h"
#include "ArticleWithImageDlg.h"

// CExerciseMainDlg dialog

class CExerciseMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CExerciseMainDlg)

public:
	CExerciseMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExerciseMainDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EM_EXERCISE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBCGPButton m_btnAbort;
	CBCGPButton m_btnBegin;
	CBCGPButton m_btnPrev;
	CBCGPButton m_btnNext;
	CBCGPButton m_btnStop;
	CBCGPButton m_btnStdAnswer;
	CBCGPButton m_btnStuAnswer;
	CBCGPButton m_btnFinish;
	CTreeCtrl m_tree;
	BOOL		m_bLoadMode;

	CString				m_strLogName;
	virtual BOOL OnInitDialog();

private:
	CLISTQUESTION m_lstQuestions;
	long				m_vaultid;
	CImageList			m_TreeNodeImages;

	CQuestionDlg*		m_pArticleDlg;
	CQuestionDlg*		m_pArticleWithImageDlg;
	CQuestionDlg*		m_pListenDlg;
	CQuestionDlg*		m_pDefaultDlg;
	CQuestionDlg*		m_pWriteDlg;
	CQuestionDlg*		m_pChoiceDlg;
	CQuestionDlg*		m_pArticleListenDlg;

	CQuestionDlg*		m_pActiveDlg;

	CExerciseQuestionRecord	m_log;
	BOOL				m_isupdateuser;
public:
	afx_msg void OnBnClickedBtnEmStuanswer();
	afx_msg void OnBnClickedBtnEmStdanswer();
public:
	afx_msg void OnNMClickTreeEmQuestion(NMHDR *pNMHDR, LRESULT *pResult);

private:
	BOOL CreateArticleDlg();
	BOOL CreateListenDlg();
	BOOL CreateWriteDlg();
	BOOL CreateChoiceDlg();
	BOOL CreateArticleListenDlg();
	BOOL CreateDefaultDlg();

	BOOL SwitchActiveDlg(CQuestionDlg* pDlg);

	BOOL ReLocation(CQuestionDlg* pDlg);
	//得到_hItem的前一个节点
	HRESULT GetPrevTreeItem(HTREEITEM _hItem,HTREEITEM &_hPrevItem);
	HRESULT GetNextTreeItem(HTREEITEM _hItem,HTREEITEM &_hNextItem);
	//当在树上点击节点时，在右边上显示对应题目
	HRESULT ShowSelItem(HTREEITEM _hItem);
public:
	afx_msg void OnBnClickedBtnEmPrev();
	afx_msg void OnBnClickedBtnEmNext();
	afx_msg void OnBnClickedBtnEmClose();
	afx_msg void OnBnClickedBtnEmRename();
};
