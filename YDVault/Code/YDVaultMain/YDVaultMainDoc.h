
// YDVaultMainDoc.h : interface of the CYDVaultMainDoc class
//


#pragma once


class CYDVaultMainDoc : public CDocument
{
protected: // create from serialization only
	CYDVaultMainDoc();
	DECLARE_DYNCREATE(CYDVaultMainDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CYDVaultMainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


