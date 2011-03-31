/*########################################################################
	��ʹ��˵����:
		1. ����������ָ�� msado15.dll ��ȫ·��, һ���� "C:\Program Files\
		   Common Files\System\ADO\" Ŀ¼��.
		2. ��ʹ�ñ���֮ǰ����Ҫ��ʼ�� COM ����, ���Ե��� CoInitialize(NULL)  
		   ����ʼ��, �� CoUninitialize() �ͷ�;
		3. ��ʹ�ü�¼������ǰ�����ȵ��� CAdoConnection::Open() �������ݿ�,
		   ���Ӻ�ɸ���� CAdoRecordSet ����ʹ��, �������ر����ݿ�.
		4. ʹ�ü�¼��ִ��SQL���֮ǰ, Ҫʹ�ù������������ SetAdoConnection() 
		   ���������Ӷ���.
########################################################################*/
#if !defined(_ANYOU_COOL_ADO_H)
#define _ANYOU_COOL_ADO_H

#if _MSC_VER > 1000
#pragma once
#endif 

#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;  
#include <icrsint.h>
class CAdoConnection;
#include "AdoRecordSet.h"
#include "AdoCommand.h"

class CAdoConnection
{
// ����/���� ------------------------------------------
public:
	CAdoConnection();
	 ~CAdoConnection();
	// ��ֵ����ת�� -----------------------------------
	static COleDateTime vartodate(const _variant_t& var);
	static COleCurrency vartocy(const _variant_t& var);
	static bool vartobool(const _variant_t& var);
	static BYTE vartoby(const _variant_t& var);
	static short vartoi(const _variant_t& var);
	static long vartol(const _variant_t& var);
	static double vartof(const _variant_t& var);
	static CString vartostr(const _variant_t& var);
protected:
	void Release();

// ���� -----------------------------------------------
public:
	// ���Ӷ��� ----------------------------------
	_ConnectionPtr& GetConnection() {return m_pConnection;};

	// �쳣��Ϣ ----------------------------------
	CString GetLastErrorText();
	ErrorsPtr GetErrors();
	ErrorPtr  GetError(long index);

	// �����ִ� ----------------------------------
	CString GetConnectionText() {return m_strConnect;}

	// ������Ϣ ----------------------------------
	CString GetProviderName();
	CString GetVersion();
	CString GetDefaultDatabase();

	// ����״̬ ----------------------------------
	BOOL IsOpen();
	long GetState();
	
	// ����ģʽ ----------------------------------
	ConnectModeEnum GetMode();
	BOOL SetMode(ConnectModeEnum mode);

	// ����ʱ�� ----------------------------------
	long GetConnectTimeOut();
	BOOL SetConnectTimeOut(long lTime = 5);
	
	// ����Դ��Ϣ -------------------------------
	_RecordsetPtr OpenSchema(SchemaEnum QueryType);

// ���� -----------------------------------------------
public:
	// ���ݿ����� --------------------------------
	BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
	BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
	BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
	BOOL OpenUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
	void Close();

// ���� -----------------------------------------------
public:
	// ������ ----------------------------------
	long BeginTrans();
	BOOL RollbackTrans();
	BOOL CommitTrans();

	// ִ�� SQL ��� ------------------------------
	_RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
	BOOL Cancel();

// ���� -----------------------------------------------
protected:
	CString			m_strConnect;
	_ConnectionPtr	m_pConnection;
};

#endif // !defined(_ANYOU_COOL_ADO_H)










































