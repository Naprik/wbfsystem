// CriticalSectionControl.cpp: implementation of the CCriticalSectionControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CriticalSectionControl.h"
#include <afxmt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCriticalSectionControl::CCriticalSectionControl(CCriticalSection* _pSec)
{
	m_pSec = _pSec;
	ASSERT(m_pSec);
	m_pSec->Lock();
}

CCriticalSectionControl::~CCriticalSectionControl()
{
	m_pSec->Unlock();
}
