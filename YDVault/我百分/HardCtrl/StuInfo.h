// StuInfo.h: interface for the CStuInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STUINFO_H__46F28B15_C902_40B8_B9EE_B08FECCBDE81__INCLUDED_)
#define AFX_STUINFO_H__46F28B15_C902_40B8_B9EE_B08FECCBDE81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStuInfo  
{
public:
	CStuInfo();
	virtual ~CStuInfo();

	CString sStuID;
	CString sStuName;
	CString sStuClass;
	CString sStuDev;
	CString sStuAnswer;

};

#endif // !defined(AFX_STUINFO_H__46F28B15_C902_40B8_B9EE_B08FECCBDE81__INCLUDED_)
