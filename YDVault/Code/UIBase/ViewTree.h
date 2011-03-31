
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree window
enum TREE_TYPE
{
	QUESTION_TREE,
	PAPER_TREE,
	EXAMOPERATE_TREE,
	DATAMAINTAIN_TREE
};
class BASE_DLL_API CViewTree : public CTreeCtrl  
{
// Construction
public:
	CViewTree();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CViewTree();
	HRESULT GetTriggerSelChange(BOOL &_bTriggerSelChange){_bTriggerSelChange = m_bTriggerSelChange;return S_OK;}
	HRESULT SetTriggerSelChange(BOOL _bTriggerSelChange){m_bTriggerSelChange = _bTriggerSelChange;return S_OK;}
	void SetTreeType(TREE_TYPE type);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
private:
	BOOL		m_bTriggerSelChange;//ÊÇ·ñ´¥·¢,OnTvnSelchanged
	TREE_TYPE	m_treeType;
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};
