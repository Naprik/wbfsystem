#pragma once
#include "YDCom.h"
#include "afxtempl.h"
#include "Studev.h"
#include <list>
#include "YDAppCom.h"
//#include "JahFile.h"
//#include "BackUp.h"
class CExamQuestionSegment;

class CStuMacState
{
public:
	CString m_strMac;//mac��ַ
	//0��δ���� 1���ɹ� 2��ʧ�� 3����ǰ�����־
	int	m_iBeginExam;//����״̬
	int m_iRolling;//�վ�״̬
};

//CYDTeacherAppCom,��ʦ��������Com����ͨ�ŵĿ���
class AFX_EXT_CLASS CYDTeacherAppCom  
	:public CYDAppCom
{
public:
	CYDTeacherAppCom(CString _strCom);
	virtual ~CYDTeacherAppCom();
public:
	virtual HRESULT    OpenCom();
	

	HRESULT    ResetSYS(); //ϵͳ��λ
	HRESULT    Resetmac();
	HRESULT    Reset(); //��λ
	HRESULT    InitSearchStu(); //���ʦ����������ָ�ʹ��ʦ����ʼ����ѧ����
	HRESULT    GetStudev(std::list<CString> &_lstStrMac); //�õ�����ڽ�ʦ��������ѧ������ַ���ַ�������
	HRESULT    Sendvalidmac(std::list<CString> &_lstStrMac);//���ʦ�����ͺϷ�ѧ������ַ
	HRESULT    TestKinkSendvalidMac(std::list<CString> &_lstStrMac);//����ģʽ�·��ͺϷ�MAC��ַ����ʦ��
	HRESULT    Sendsinglemac(CString mac,int index,int& _iRetrun); //��������ģʽ�����ͽ���ָ�
																 //0x89 00 00 04 + ѧ�����+ 0d 0d 0a
// 	HRESULT    UpdateStudev(int index, CString strMac);
 	HRESULT    Addmac(int index, CString mac); //�������ĵ�ַ�����ʦ���Ϸ��ĵ�ַ��

	virtual		HRESULT Sendexaminfo();
	//�ݲ�֪��
	HRESULT    Sendoffexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO);

	//�������Ϳ�����Ϣ
	HRESULT    SendSingleinfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO);
	//��ʧ�ܵĿ������·��Ϳ�����Ϣ��
	HRESULT    Sendstepexaminfo(CArray<CStudev, CStudev>  &arry);
	//�����б���ѧ��״̬��־λ
	HRESULT    Updatestat( BOOL &bRoll, BOOL &bRolling);
	//ֱ�Ӷ�ȡ��ʦ���е�ѧ������Ϣ�����һ����ɽ���浽m_lstStuDev��
	//��ֻ�Ƕ�ȡ��ʦ����ʱ����
	HRESULT    UpdateOnlyFromTeacher();
// 	//���½�ʦ���еĽ���״̬��lstRoolingMac����ǳɹ���ȡ�𰸵�ѧ����Mac��ַ
// 	HRESULT    UpdateStateRolling(BOOL &bRooling,std::list<CString> &lstRoolingMac);
	//�մ�
	HRESULT    Rolling(std::list<CExamQuestionSegment*> &_lstQSeg);
	//��ȡ����״̬
	HRESULT    IsRolldone(std::list<CStudev*> &_lstStu);
	//��ȡ�վ�״̬
	HRESULT    IsRollingdone(std::list<CStudev*> &_lstStu);
	//��ѧ������ǰ����ģ����е����վ�
	HRESULT    PreRolling(int index);

	HRESULT    OpenFM();//��������
	HRESULT    SearchFM();//����������
	HRESULT    CloseFM();//�ر�
	HRESULT    UpFM();//
	HRESULT    DownFM();
	HRESULT    SetFM(CString strFM);
	HRESULT    GetFM(CString &_strFM);
	HRESULT    SingleRolling(int index); //�����վ�
	HRESULT    SendFirstMac(CString strMac); //���ͺϷ�mac��ַ

	//��ȡ��
	HRESULT    Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO);
	//���Ϸ�ѧ�������������ַ��Ӧ��ϵ
	HRESULT    Getrelationmap(CStringArray &arryrelation);
	
	HRESULT    Getanswer(	std::list<CExamQuestionSegment*> &_lstQSeg, 
							CString &strAnswerout);
	HRESULT    GetSingleanswer(int index, int iStartNO, int iEndNO, CString &strAnswerout);
	//����ѧ����MAC��ַ�õ������ѧ���Ĵ�,���Ҹ���״̬
	HRESULT    GetAnswerByStudev(CStudev* pStudev,
								int iStartNO, int iEndNO,
							  CString& _strAnswer);

	HRESULT    FetchAnswer(CString &_str);

	HRESULT    InrarSendExamInfo();//�ں���ģʽ�£����ʦ�����Ϳ�����Ϣ
	HRESULT    BeginExamSingle(CString _strMac);//��_strMac������������

protected:
	BOOL				isSearch;
	CString				 m_path;
	//_strEncode�Ǽ��ܵ��ַ������������Ϊ���ĵĴ�
	HRESULT	  DecodeAnswer(CString _strEncode,int iStartNO, int iEndNO,
									CString &_strAnswer);
	//�����ַ����������õ�ѧ������Mac��ַ����Ӧ��״̬
	//_str�ַ�����ʽ���£�01 86 58 00 00 01 86 57 00 00...
	//���У�01 86 58����Mac��ַ��00 00 ���ֱ�������״̬�ͽ���״̬
	HRESULT   ParseStuStr(CString _str,std::list<CStuMacState*> &_lstStuMac);
public:
	std::list<CStudev*>  m_lstStuDev;//��ǰ��ʦ����Ӧ�ĵ�����ѧ����

public:
	//�õ����еĽ�ʦ��Com�˿�
	static		HRESULT FindTeacherCom();
};


