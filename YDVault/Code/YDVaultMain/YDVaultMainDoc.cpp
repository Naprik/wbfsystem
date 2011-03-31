
// YDVaultMainDoc.cpp : implementation of the CYDVaultMainDoc class
//

#include "stdafx.h"
#include "YDVaultMain.h"

#include "YDVaultMainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYDVaultMainDoc

IMPLEMENT_DYNCREATE(CYDVaultMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CYDVaultMainDoc, CDocument)
END_MESSAGE_MAP()


// CYDVaultMainDoc construction/destruction

CYDVaultMainDoc::CYDVaultMainDoc()
{
	// TODO: add one-time construction code here

}

CYDVaultMainDoc::~CYDVaultMainDoc()
{
}

BOOL CYDVaultMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CYDVaultMainDoc serialization

void CYDVaultMainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CYDVaultMainDoc diagnostics

#ifdef _DEBUG
void CYDVaultMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CYDVaultMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CYDVaultMainDoc commands
