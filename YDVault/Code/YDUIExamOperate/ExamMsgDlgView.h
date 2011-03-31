#pragma once
#include "../UIBase/ViewTree.h"
#include "../UIBase\ListBoxEx.h"
class CYdObjWrapper;



class  CExamMsgDlgViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class BASE_DLL_API CExamMsgDlgView : public CDockablePane
{
	
public:
	CExamMsgDlgView();
	virtual ~CExamMsgDlgView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	CListBoxEx	m_lstBoxmsg;
protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMIDelMsg();
	afx_msg void OnMIClearMsg();
	afx_msg void OnMICopyMsg();


private:
	std::list<CYdObjWrapper*> m_lstClear;//用于释放
	HRESULT CopyToClipboard(CString& strVal);//将strVal得值放到剪切板中
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


