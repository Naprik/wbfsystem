#pragma once
#include "..\objref\ydobjectref.h"
#include "..\DBBase\DatabaseEx.h"

class BASE_DLL_API CYDDevice :
	public CYDObjectRef
{
public:
	CYDDevice(CDatabaseEx* pDb);
	~CYDDevice(void);

	HRESULT GetDeviceMAC(const CString& strID, CString* pStrMac);
	HRESULT GetDeviceID(const CString& strMac, CString* pStrID);
	virtual HRESULT Remove();
protected:
	CYDObjectRef* CreateObject();
	HRESULT GetInAddrUnit(std::list<CYDObjectRef*>& _lstAddrUnit);
};
