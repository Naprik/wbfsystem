#pragma once

class AFX_EXT_CLASS CButtonItem : public CBCGPGridItem
{
public:
	//��_pWnd����click��Ϣ�����_pWnd=NULL,����ǰ�ĸ���CBCGPGridCtrl������Ϣ
	CButtonItem (LPCTSTR lpszText, UINT id,CWnd* _pWnd = NULL);

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
};