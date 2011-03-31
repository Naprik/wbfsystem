#pragma once



#define	ROLLBACK_AND_RETURN(hr, transaction) \
	do \
	{ \
		HRESULT hrTmp = E_FAIL; \
		hrTmp = transaction.Rollback(); \
		if (FAILED(hrTmp)) return hrTmp; \
		return hr; \
	} while(0);

class CDBTransactionRef;
class CDatabaseEx;

class BASE_DLL_API CDBTransactionContext
{
public:
	CDBTransactionContext(void){m_pTrans=NULL;}
	virtual ~CDBTransactionContext(void){}

	CDBTransactionRef* CurrentTransaction(){return m_pTrans;}
	void SetCurrentTransaction(CDBTransactionRef* p){m_pTrans=p;}
protected:
	CDBTransactionRef* m_pTrans;
};

class BASE_DLL_API CDBTransactionRef
{
public:
	CDBTransactionRef(CDatabaseEx* _pDB,BOOL bAutoTransaction=FALSE);
	virtual ~CDBTransactionRef(void);

	virtual HRESULT Begin();
	virtual HRESULT Commit();
	virtual HRESULT Rollback();

public:
BOOL IsBegin(){return m_bBegin;}
	static CDBTransactionContext* GetContext();
protected:
	static CDBTransactionContext m_context;
	BOOL                          m_bBegin;
	CDatabaseEx*                  m_pDB;
};
