#include "StdAfx.h"
#include "SearchDevCom.h"

CSearchDevCom::CSearchDevCom(void)
{
}

CSearchDevCom::~CSearchDevCom(void)
{
}

HRESULT CSearchDevCom::SearchDev(std::list<CString> &_lstCom)
{
	HRESULT hr = E_FAIL;
	HKEY hKey;

	if(::RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"),
		NULL,
		KEY_READ,
		&hKey ) == ERROR_SUCCESS )
	{
		int i=0;
		TCHAR portName[256],commName[256];
		DWORD dwLong,dwSize;

		while(true)
		{
			dwLong = dwSize = sizeof(portName);
			if(::RegEnumValue( hKey,
				i,
				portName,
				&dwLong,
				NULL,
				NULL,
				(PUCHAR)commName,
				&dwSize ) == ERROR_NO_MORE_ITEMS )// Ã¶¾Ù´®¿Ú
				break;
			i++;
			CString strPortName = portName;
			if(strPortName.Find(_T("Serial")) >= 0)
			{
				CString strPort = commName;
				_lstCom.push_back(strPort);
			}
		}
	}
	return S_OK;
}
