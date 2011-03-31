
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "QuestionTreeView.h"
#include "PaperTreeView.h"
#include "../YDUIExamOperate/ExamTreeView.h"
#include "../YDUIExamDataMaintain/DataMaintainTreeView.h"
#include "../YDUIExamOperate\ExamMsgDlgView.h"

class CCfgTree;

class CLanguageTranslator;
class CObjPropSheetManager;
class CObjPropertySheet;


class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CQuestionTreeView m_wndQuestionTreeView;
	CPaperTreeView    m_wndPaperTreeView;
	CExamTreeView	  m_wndExamTreeView;
	CDataMaintainTreeView m_wndDataMaintainTreeView;

	CExamMsgDlgView	  m_wndMsgDlgView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	afx_msg	HRESULT OnOpenObject(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnOpenOBjectEX(WPARAM wParam, LPARAM lParam);
	afx_msg	HRESULT OnOpenDocument(WPARAM wParam, LPARAM lParam);
	afx_msg	HRESULT OnOpenDocumentEX(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnGetTranslator(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnGetPropSheetManager(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnCloseAllPropSheet(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnClosePropSheet(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnGetFtpRef(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnGetDB(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnGetBHiColorIcons(WPARAM wParam,LPARAM);

	afx_msg HRESULT OnAddPage(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnGetExamMsgListBox(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnGetLastSheet(WPARAM wParam,LPARAM lParam);

	//////////////////////////////////////////////////////////////////////////
	//Exam
	//打开院系信息
	afx_msg HRESULT OnOpenExamDepartment(WPARAM wParam, LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////

	afx_msg HRESULT OnTeacherComBreak(WPARAM wParam, LPARAM lParam);


private:
	BOOL CreateDockingWindows(CCfgTree* _pCfgTree);
	void SetDockingWindowIcons(CCfgTree* _pCfgTree,BOOL bHiColorIcons);
private:
	CLanguageTranslator* m_pTranslator;
	CObjPropSheetManager* m_pPropSheetManager;
	CObjPropertySheet*     m_pLastSheet;//最近一个打开的Sheet
public:
	afx_msg void OnClose();
};


