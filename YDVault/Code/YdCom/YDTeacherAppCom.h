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
	CString m_strMac;//mac地址
	//0：未连接 1：成功 2：失败 3：提前交卷标志
	int	m_iBeginExam;//开考状态
	int m_iRolling;//收卷状态
};

//CYDTeacherAppCom,教师机用来与Com进行通信的控制
class AFX_EXT_CLASS CYDTeacherAppCom  
	:public CYDAppCom
{
public:
	CYDTeacherAppCom(CString _strCom);
	virtual ~CYDTeacherAppCom();
public:
	virtual HRESULT    OpenCom();
	

	HRESULT    ResetSYS(); //系统复位
	HRESULT    Resetmac();
	HRESULT    Reset(); //复位
	HRESULT    InitSearchStu(); //向教师机发送搜索指令，使教师机开始搜索学生机
	HRESULT    GetStudev(std::list<CString> &_lstStrMac); //得到存放在教师机搜索到学生机地址的字符串数组
	HRESULT    Sendvalidmac(std::list<CString> &_lstStrMac);//向教师机发送合法学生机地址
	HRESULT    TestKinkSendvalidMac(std::list<CString> &_lstStrMac);//测试模式下发送合法MAC地址到教师机
	HRESULT    Sendsinglemac(CString mac,int index,int& _iRetrun); //单独点名模式，发送建链指令：
																 //0x89 00 00 04 + 学生编号+ 0d 0d 0a
// 	HRESULT    UpdateStudev(int index, CString strMac);
 	HRESULT    Addmac(int index, CString mac); //将新增的地址加入教师机合法的地址中

	virtual		HRESULT Sendexaminfo();
	//暂不知道
	HRESULT    Sendoffexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO);

	//单独发送考试信息
	HRESULT    SendSingleinfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO);
	//对失败的考生重新发送考试信息包
	HRESULT    Sendstepexaminfo(CArray<CStudev, CStudev>  &arry);
	//更新列表中学生状态标志位
	HRESULT    Updatestat( BOOL &bRoll, BOOL &bRolling);
	//直接读取教师机中的学生机信息，并且会生成结果存到m_lstStuDev中
	//当只是读取教师机答案时调用
	HRESULT    UpdateOnlyFromTeacher();
// 	//更新教师机中的交卷状态，lstRoolingMac存的是成功获取答案的学术界Mac地址
// 	HRESULT    UpdateStateRolling(BOOL &bRooling,std::list<CString> &lstRoolingMac);
	//收答案
	HRESULT    Rolling(std::list<CExamQuestionSegment*> &_lstQSeg);
	//获取点名状态
	HRESULT    IsRolldone(std::list<CStudev*> &_lstStu);
	//获取收卷状态
	HRESULT    IsRollingdone(std::list<CStudev*> &_lstStu);
	//对学生机提前交卷的，进行单独收卷
	HRESULT    PreRolling(int index);

	HRESULT    OpenFM();//打开收音机
	HRESULT    SearchFM();//搜索收音机
	HRESULT    CloseFM();//关闭
	HRESULT    UpFM();//
	HRESULT    DownFM();
	HRESULT    SetFM(CString strFM);
	HRESULT    GetFM(CString &_strFM);
	HRESULT    SingleRolling(int index); //单独收卷
	HRESULT    SendFirstMac(CString strMac); //发送合法mac地址

	//获取答案
	HRESULT    Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO);
	//读合法学生编号与蓝牙地址对应关系
	HRESULT    Getrelationmap(CStringArray &arryrelation);
	
	HRESULT    Getanswer(	std::list<CExamQuestionSegment*> &_lstQSeg, 
							CString &strAnswerout);
	HRESULT    GetSingleanswer(int index, int iStartNO, int iEndNO, CString &strAnswerout);
	//根据学生机MAC地址得到，这个学生的答案,并且更新状态
	HRESULT    GetAnswerByStudev(CStudev* pStudev,
								int iStartNO, int iEndNO,
							  CString& _strAnswer);

	HRESULT    FetchAnswer(CString &_str);

	HRESULT    InrarSendExamInfo();//在红外模式下，向教师机发送考试信息
	HRESULT    BeginExamSingle(CString _strMac);//对_strMac单独蓝牙开考

protected:
	BOOL				isSearch;
	CString				 m_path;
	//_strEncode是加密的字符串，将其解密为明文的答案
	HRESULT	  DecodeAnswer(CString _strEncode,int iStartNO, int iEndNO,
									CString &_strAnswer);
	//解析字符串，分析得到学生机的Mac地址及对应的状态
	//_str字符串格式如下：01 86 58 00 00 01 86 57 00 00...
	//其中：01 86 58代表Mac地址，00 00 ：分别代表点名状态和交卷状态
	HRESULT   ParseStuStr(CString _str,std::list<CStuMacState*> &_lstStuMac);
public:
	std::list<CStudev*>  m_lstStuDev;//当前教师机对应的点名过学生机

public:
	//得到所有的教师机Com端口
	static		HRESULT FindTeacherCom();
};


