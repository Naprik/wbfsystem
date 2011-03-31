#pragma once
#include "ydobjectref.h"

class BASE_DLL_API CYDMediaRef :
	public CYDObjectRef
{
public:
	CYDMediaRef(CDatabaseEx* pDB);
	~CYDMediaRef(void);

public:
	virtual HRESULT GetLabel(CString* pStr);
	HRESULT CreateVaultName(CString& _strVaultName);//得到在库中所存的文件名,/Vault/+MP3/GUID文件名
	virtual HRESULT Remove();
};
