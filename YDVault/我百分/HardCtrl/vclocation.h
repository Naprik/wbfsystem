#if !defined(AFX_VCLOCATION_H__4210EF03_10B6_4EAD_8355_68E427CBA554__INCLUDED_)
#define AFX_VCLOCATION_H__4210EF03_10B6_4EAD_8355_68E427CBA554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcRect;

/////////////////////////////////////////////////////////////////////////////
// CVcLocation wrapper class

class CVcLocation : public COleDispatchDriver
{
public:
	CVcLocation() {}		// Calls COleDispatchDriver default constructor
	CVcLocation(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcLocation(const CVcLocation& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CVcRect GetRect();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetLocationType();
	void SetLocationType(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCLOCATION_H__4210EF03_10B6_4EAD_8355_68E427CBA554__INCLUDED_)
