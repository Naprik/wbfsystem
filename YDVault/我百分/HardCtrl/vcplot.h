#if !defined(AFX_VCPLOT_H__B52D7C47_1E36_4319_A5BC_73C0051F28EC__INCLUDED_)
#define AFX_VCPLOT_H__B52D7C47_1E36_4319_A5BC_73C0051F28EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcBackdrop;
class CVcRect;
class CVcView3d;
class CVcPlotBase;
class CVcWeighting;
class CVcWall;
class CVcSeriesCollection;
class CVcLight;
class CVcAxis;

/////////////////////////////////////////////////////////////////////////////
// CVcPlot wrapper class

class CVcPlot : public COleDispatchDriver
{
public:
	CVcPlot() {}		// Calls COleDispatchDriver default constructor
	CVcPlot(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcPlot(const CVcPlot& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetAngleUnit();
	void SetAngleUnit(long nNewValue);
	BOOL GetClockwise();
	void SetClockwise(BOOL bNewValue);
	long GetSort();
	void SetSort(long nNewValue);
	CVcBackdrop GetBackdrop();
	float GetStartingAngle();
	void SetStartingAngle(float newValue);
	long GetSubPlotLabelPosition();
	void SetSubPlotLabelPosition(long nNewValue);
	float GetBarGap();
	void SetBarGap(float newValue);
	float GetXGap();
	void SetXGap(float newValue);
	BOOL GetAutoLayout();
	void SetAutoLayout(BOOL bNewValue);
	float GetWidthToHeightRatio();
	void SetWidthToHeightRatio(float newValue);
	float GetDepthToHeightRatio();
	void SetDepthToHeightRatio(float newValue);
	CVcRect GetLocationRect();
	long GetProjection();
	void SetProjection(long nNewValue);
	float GetZGap();
	void SetZGap(float newValue);
	CVcView3d GetView3d();
	CVcPlotBase GetPlotBase();
	CVcWeighting GetWeighting();
	CVcWall GetWall();
	CVcSeriesCollection GetSeriesCollection();
	BOOL GetDataSeriesInRow();
	void SetDataSeriesInRow(BOOL bNewValue);
	CVcLight GetLight();
	long GetDefaultPercentBasis();
	BOOL GetUniformAxis();
	void SetUniformAxis(BOOL bNewValue);
	CVcAxis GetAxis(long axisID, const VARIANT& Index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCPLOT_H__B52D7C47_1E36_4319_A5BC_73C0051F28EC__INCLUDED_)
