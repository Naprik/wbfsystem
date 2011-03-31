#pragma once



// CYdConfigSheet
#include "YdConfigDBPage.h"
#include "YdConfigFtpPage.h"

#include "../DBBase/DBConfig.h"
#include "../FtpBase/FtpConfig.h"

class CYdConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CYdConfigSheet)

public:
	CYdConfigSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CYdConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CYdConfigSheet();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CYdConfigDBPage		m_pageConfigDB;
	CYdConfigFtpPage	m_pageConfigFtp;
public:
	HRESULT             Save();
	CDBConfig			m_DBConfig;
	CFtpConfig			m_FtpConfig;
};


