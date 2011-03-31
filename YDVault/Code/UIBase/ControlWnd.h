#pragma once

//在构造函数时将m_pWnd->EnableWindow False
//析构函数时m_pWnd->EnableWindow True
class AFX_EXT_CLASS CControlWnd//用来控制Wnd
{
public:
	CControlWnd(CWnd* _pWnd);
	~CControlWnd(void);
private:
	CWnd*	m_pWnd;
	BOOL	m_bOld;
};
