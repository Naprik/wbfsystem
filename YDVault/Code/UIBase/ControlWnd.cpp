#include "StdAfx.h"
#include "ControlWnd.h"

CControlWnd::CControlWnd(CWnd* _pWnd)
{
	m_pWnd = _pWnd;
	ASSERT(m_pWnd);
	m_bOld = m_pWnd->IsWindowEnabled();
	m_pWnd->EnableWindow(FALSE);
}

CControlWnd::~CControlWnd(void)
{
	m_pWnd->EnableWindow(m_bOld);
}
