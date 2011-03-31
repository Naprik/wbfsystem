#pragma once
class CXmlWriter;
class CYDExamInfo;
class CYDEAddrUnit;
class CYDPaper;
class CYDStuMark;

class CInvigilateInfo //考试的监考信息
{
public:
	CInvigilateInfo(void);
	~CInvigilateInfo(void);
public:
	HRESULT Save(BSTR _strFile);
	HRESULT Load(BSTR _strFile);
	HRESULT SaveInvigilateDB(CDatabaseEx* _pDB);//将当前考试的信息存到数据库中
	HRESULT SaveAnswer(CDatabaseEx* _pDB,
					   CYDEAddrUnit* _pUint,
					   CYDObjectRef* _pPaper,
					   CYDObjectRef* _pSubject,
					   CString _strAnswer);//存入当前学生的考试成绩
	HRESULT IsExist(CDatabaseEx* _pDB,VARIANT_BOOL* _bExist);//判断当前的监考信息是否在数据库中，通过监考信息文件名
private:
	//保存点名，收卷模式和考试性质
	HRESULT SaveModeInfo(CXmlWriter &_writer,CGExamStruct* _pGExamStruct);
	//保存听力
	HRESULT SaveExamListen(CXmlWriter &_writer,CGExamStruct* _pGExamStruct);
	//保存答案
	HRESULT SaveExamAnswer(CXmlWriter &_writer,CGExamStruct* _pGExamStruct);

	//读取点名，收卷模式和考试性质
	HRESULT LoadModeInfo(CXmlNode& _node,CGExamStruct* _pGExamStruct);
	//读取班级信息
	HRESULT LoadClass(CXmlNode& _node,CGExamStruct* _pGExamStruct,CDatabaseEx* pDB);
	//读取考场单元
	HRESULT LoadExamRoomUnit(CXmlNode& _node,CGExamStruct* _pGExamStruct,CDatabaseEx* pDB);
	//考试时间
	HRESULT LoadExamTime(CXmlNode& _node,CGExamStruct* _pGExamStruct);
	//读取听力设置
	HRESULT LoadExamListen(CXmlNode& _node,CGExamStruct* _pGExamStruct);
	//答案
	HRESULT LoadExamAnswer(CXmlNode& _node,CGExamStruct* _pGExamStruct);

	//根据全局变量，创建一个监考信息对象
	HRESULT CreateExamInfoObj(CGExamStruct* _pGExamStruct,CDatabaseEx* _pDB);
	//在xml中只存了时间，所以load的时候要特殊处理一下
	HRESULT LoadTime(COleDateTime& _time,CComBSTR& _bstrVal);
private:
	CYDExamInfo*				m_pExamInfo;
	std::list<CYDStuMark*>		m_lstStuMark;//学生成绩
	CString						m_strXmlFile;
};
