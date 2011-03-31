#pragma once
#include "YDStuAppCom.h"

class CStudentCom
{
public:
	CStudentCom(void);
	~CStudentCom(void);
public:
	HRESULT       Opencom(CString  strCOM, CString strMac); //打开Com接口
	HRESULT       Closecom();                               //关闭Com接口
	HRESULT       Get_Comstat_text(CString &_str);          //获取当前Com接口是否使用的状态
public:
	HRESULT       LinkRolling(int iStart, int iEnd,CString &_str);
	HRESULT       LinkSuccess();
private:
	CYDStuAppCom		m_com;       //连接当前学生机的COM接口，直接通过USB连接时
};
