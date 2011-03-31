#if !defined(AFX_VCVALUESCALE_H__CF5E6395_9E52_4A42_A528_BBB08FD2C25E__INCLUDED_)
#define AFX_VCVALUESCALE_H__CF5E6395_9E52_4A42_A528_BBB08FD2C25E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CVcValueScale wrapper class

class CVcValueScale : public COleDispatchDriver
{
public:
	CVcValueScale() {}		// Calls COleDispatchDriver default constructor
	CVcValueScale(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcValueScale(const CVcValueScale& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetAuto();
	void SetAuto(BOOL bNewValue);
	double GetMinimum();
	void SetMinimum(double newValue);
	double GetMaximum();
	void SetMaximum(double newValue);
	short GetMajorDivision();
	void SetMajorDivision(short nNewValue);
	short GetMinorDivision();
	void SetMinorDivision(short nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCVALUESCALE_H__CF5E6395_9E52_4A42_A528_BBB08FD2C25E__INCLUDED_)
