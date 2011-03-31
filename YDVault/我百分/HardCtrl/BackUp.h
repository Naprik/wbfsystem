// BackUp.h: interface for the CBackUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKUP_H__2B3C8DEB_61D7_4E2F_9263_8D3894BED646__INCLUDED_)
#define AFX_BACKUP_H__2B3C8DEB_61D7_4E2F_9263_8D3894BED646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JahFile.h"

class CBackUp  
{
public:
	CBackUp();
	virtual ~CBackUp();

	void			Backup(CString strTitle, CString strDetails);
	CString			Load(CString strTitle, CString strFilename);
	CString			Load(CString strTitle, CString strKey, CString strFilename);
	void			Createfile(CString strTitle, CString strDetails);
	void			Createfile(CString strTitle, CString strKey, CString strDetails);
	void			Remove();
	void			Remove(BOOL flag);
private:
	CString   m_filepath;
	
};

#endif // !defined(AFX_BACKUP_H__2B3C8DEB_61D7_4E2F_9263_8D3894BED646__INCLUDED_)
