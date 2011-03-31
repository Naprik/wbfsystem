// CriticalSectionControl.h: interface for the CCriticalSectionControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSECTIONCONTROL_H__26995C1A_AFFC_4E61_9584_0B529DEB0A08__INCLUDED_)
#define AFX_CRITICALSECTIONCONTROL_H__26995C1A_AFFC_4E61_9584_0B529DEB0A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CCriticalSection;

class BASE_DLL_API CCriticalSectionControl  
{
public:
	CCriticalSectionControl(CCriticalSection* _pSec);
	virtual ~CCriticalSectionControl();
private:
	CCriticalSection* m_pSec;
};

#endif // !defined(AFX_CRITICALSECTIONCONTROL_H__26995C1A_AFFC_4E61_9584_0B529DEB0A08__INCLUDED_)
