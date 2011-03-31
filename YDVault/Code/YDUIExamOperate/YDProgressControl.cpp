#include "StdAfx.h"
#include "YDProgressControl.h"

CYDProgressControl::CYDProgressControl(CWnd* _pWnd,BOOL bShowCancel /*= FALSE*/)
{
	ASSERT(_pWnd);
	m_pWnd = _pWnd;
	m_pWnd->SendMessage(WM_PROGRESSCTRL_SHOW,SW_SHOW);
	if(bShowCancel)
	{
		m_pWnd->SendMessage(WM_PROGRESSCTRL_SHOWCANCEL,SW_SHOW);
	}
}

CYDProgressControl::~CYDProgressControl(void)
{
	m_pWnd->SendMessage(WM_PROGRESSCTRL_SHOW,SW_HIDE);
	m_pWnd->SendMessage(WM_PROGRESSCTRL_SHOWCANCEL,SW_HIDE);
}

HRESULT CYDProgressControl::SetRange(UINT uRange,BOOL bTime /*= FALSE*/)
{
	return m_pWnd->SendMessage(WM_PROGRESSCTRL_RANGE,uRange,bTime);
}
HRESULT CYDProgressControl::StepIt()
{
	return m_pWnd->SendMessage(WM_PROGRESSCTRL_SETIP);
}
