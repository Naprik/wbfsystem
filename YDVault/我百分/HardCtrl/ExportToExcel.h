// ExportToExcel.h: interface for the CExportToExcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPORTTOEXCEL_H__8B3B9837_1924_4BD0_96B8_464BB2FB8297__INCLUDED_)
#define AFX_EXPORTTOEXCEL_H__8B3B9837_1924_4BD0_96B8_464BB2FB8297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExportToExcel  
{
public:
	CExportToExcel();
	virtual ~CExportToExcel();

	void		ExportListToExcel(CListCtrl* pList, CString strTitle);


private:
	CString		GetExcelDriver();
	BOOL		MakeSurePathExists( CString &Path,	bool FilenameIncluded=true);
	BOOL        GetDefaultXlsFileName(CString& sExcelFile);
};

#endif // !defined(AFX_EXPORTTOEXCEL_H__8B3B9837_1924_4BD0_96B8_464BB2FB8297__INCLUDED_)
