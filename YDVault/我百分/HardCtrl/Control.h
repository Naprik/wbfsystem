// Control.h: interface for the CControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROL_H__AAC13F9D_4A00_41AC_84E7_33A231B697F8__INCLUDED_)
#define AFX_CONTROL_H__AAC13F9D_4A00_41AC_84E7_33A231B697F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CControl  
{
public:
	CControl();
	virtual ~CControl();
	void Create(int c,UINT ID1,UINT ID2,CDC * d,int x,int y,int w);
	CDC * GetDC(int x);
	int GetX(){return px;}
	int GetY(){return py;}
	BOOL InIt(int x,int y);
private:
	int con;
	int px,py,width;
	CDC * dc1, * dc2;
};

#endif // !defined(AFX_CONTROL_H__AAC13F9D_4A00_41AC_84E7_33A231B697F8__INCLUDED_)
