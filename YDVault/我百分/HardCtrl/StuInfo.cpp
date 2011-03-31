// StuInfo.cpp: implementation of the CStuInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hardctrl.h"
#include "StuInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStuInfo::CStuInfo()
{
	sStuID = "";
	sStuName = "";
	sStuClass = "";
	sStuDev = "";
	sStuAnswer = "";
}

CStuInfo::~CStuInfo()
{

}
