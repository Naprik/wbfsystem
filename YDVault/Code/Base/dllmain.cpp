// dllmain.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR	 _ModulePath[MAX_PATH] = {0};
static AFX_EXTENSION_MODULE BaseDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("Base.DLL Initializing!\n");
		DWORD nret = GetModuleFileName(hInstance,_ModulePath,MAX_PATH);
		_ModulePath[nret] = 0;
		TCHAR* p = _tcsrchr(_ModulePath,_T('\\'));
		if(p!=NULL) *p = 0;
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(BaseDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(BaseDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Base.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(BaseDLL);
	}
	return 1;   // ok
}
