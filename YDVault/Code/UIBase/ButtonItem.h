#pragma once

class AFX_EXT_CLASS CButtonItem : public CBCGPGridItem
{
public:
	//向_pWnd发送click消息，如果_pWnd=NULL,则向当前的父即CBCGPGridCtrl发送消息
	CButtonItem (LPCTSTR lpszText, UINT id,CBCGPGridCtrl* _pGridCtrl,
				CBCGPGridRow* _pRow,CWnd* _pWnd = NULL);

protected:
	virtual CWnd* CreateInPlaceEdit (CRect /*rectEdit*/, BOOL& /*bDefaultFormat*/)
	{
		return NULL;
	}

	virtual BOOL OnSetCursor () const
	{
		return FALSE; /* Use default */	
	}

	virtual void OnDrawValue (CDC* pDC, CRect rect);
	virtual BOOL OnClickValue (UINT uiMsg, CPoint point);

	UINT	m_id;
	CWnd*	m_pSendWnd;
	CBCGPGridCtrl* m_pGridCtrl;
	CBCGPGridRow* m_pRow;//当前Button所在的行
};