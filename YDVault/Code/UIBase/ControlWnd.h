#pragma once

//�ڹ��캯��ʱ��m_pWnd->EnableWindow False
//��������ʱm_pWnd->EnableWindow True
class AFX_EXT_CLASS CControlWnd//��������Wnd
{
public:
	CControlWnd(CWnd* _pWnd);
	~CControlWnd(void);
private:
	CWnd*	m_pWnd;
	BOOL	m_bOld;
};
