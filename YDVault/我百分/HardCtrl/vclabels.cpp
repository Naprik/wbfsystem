// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "vclabels.h"

// Dispatch interfaces referenced by this interface
#include "vclabel.h"


/////////////////////////////////////////////////////////////////////////////
// CVcLabels properties

/////////////////////////////////////////////////////////////////////////////
// CVcLabels operations

long CVcLabels::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CVcLabel CVcLabels::GetItem(short LabelIndex)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		LabelIndex);
	return CVcLabel(pDispatch);
}
