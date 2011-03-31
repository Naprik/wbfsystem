#pragma once
#include "YDStuAppCom.h"

class CStudentCom
{
public:
	CStudentCom(void);
	~CStudentCom(void);
public:
	HRESULT       Opencom(CString  strCOM, CString strMac); //��Com�ӿ�
	HRESULT       Closecom();                               //�ر�Com�ӿ�
	HRESULT       Get_Comstat_text(CString &_str);          //��ȡ��ǰCom�ӿ��Ƿ�ʹ�õ�״̬
public:
	HRESULT       LinkRolling(int iStart, int iEnd,CString &_str);
	HRESULT       LinkSuccess();
private:
	CYDStuAppCom		m_com;       //���ӵ�ǰѧ������COM�ӿڣ�ֱ��ͨ��USB����ʱ
};
