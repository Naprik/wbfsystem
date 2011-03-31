#pragma once

class CWorkSpaceMgr
{
private:
	CWorkSpaceMgr(void);
	~CWorkSpaceMgr(void);
public:
	HRESULT Load(const CString& strPath);
	HRESULT Save();

	HRESULT SetWorkspace(const CString& strWorkspace);
	HRESULT GetWorkSpace(CString* pStrWorkspace) const;
	
	static CWorkSpaceMgr* Instance(){return &s_Instance;}

private:
	CString			m_strPath;
	CString			m_strWorkspace;

	static CWorkSpaceMgr	s_Instance;
};
