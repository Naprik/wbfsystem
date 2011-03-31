// Control.cpp: implementation of the CControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Control.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControl::CControl(){
	dc1 = NULL;
	dc2 = NULL;
	con = 0;
}

CControl::~CControl(){
	if(dc1)
		delete dc1;
	if(dc2)
		delete dc2;
}

void CControl::Create(int c,UINT ID1,UINT ID2,CDC * d,int x,int y,int w){
	con = c;
	px = x;
	py = y;
	width = w;
	CBitmap * bmp,* obp;
	bmp = new CBitmap;
	bmp->LoadBitmap(ID1);
	dc1 = new CDC;
	dc1->CreateCompatibleDC(d);
	obp = dc1->SelectObject(bmp);
	obp->DeleteObject();
	bmp = new CBitmap;
	bmp->LoadBitmap(ID2);
	dc2 = new CDC;
	dc2->CreateCompatibleDC(d);
	obp = dc2->SelectObject(bmp);
	obp->DeleteObject();
}

CDC * CControl::GetDC(int x){
	if(x != con){
		return dc1;
	}
	else{
		return dc2;
	}
}

BOOL CControl::InIt(int x,int y){
	if(x>=px&&x<px+width&&y>=py&&y<py+width){
		return TRUE;
	}
	else{
		return FALSE;
	}
}