#pragma once
#include "afxwin.h"
#include "../Base\log.h"

class AFX_EXT_CLASS CListBoxEx :
	public CListBox
{
public:
	CListBoxEx(void);
	~CListBoxEx(void);
public:
	virtual int AddString(LPCTSTR lpszItem);
	CLog		m_log;
};
