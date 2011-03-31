#pragma once
class CXmlWriter;
class CYDExamInfo;
class CYDEAddrUnit;
class CYDPaper;
class CYDStuMark;

class CInvigilateInfo //���Եļ࿼��Ϣ
{
public:
	CInvigilateInfo(void);
	~CInvigilateInfo(void);
public:
	HRESULT Save(BSTR _strFile);
	HRESULT Load(BSTR _strFile);
	HRESULT SaveInvigilateDB(CDatabaseEx* _pDB);//����ǰ���Ե���Ϣ�浽���ݿ���
	HRESULT SaveAnswer(CDatabaseEx* _pDB,
					   CYDEAddrUnit* _pUint,
					   CYDObjectRef* _pPaper,
					   CYDObjectRef* _pSubject,
					   CString _strAnswer);//���뵱ǰѧ���Ŀ��Գɼ�
	HRESULT IsExist(CDatabaseEx* _pDB,VARIANT_BOOL* _bExist);//�жϵ�ǰ�ļ࿼��Ϣ�Ƿ������ݿ��У�ͨ���࿼��Ϣ�ļ���
private:
	//����������վ�ģʽ�Ϳ�������
	HRESULT SaveModeInfo(CXmlWriter &_writer,CGExamStruct* _pGExamStruct);
	//��������
	HRESULT SaveExamListen(CXmlWriter &_writer,CGExamStruct* _pGExamStruct);
	//�����
	HRESULT SaveExamAnswer(CXmlWriter &_writer,CGExamStruct* _pGExamStruct);

	//��ȡ�������վ�ģʽ�Ϳ�������
	HRESULT LoadModeInfo(CXmlNode& _node,CGExamStruct* _pGExamStruct);
	//��ȡ�༶��Ϣ
	HRESULT LoadClass(CXmlNode& _node,CGExamStruct* _pGExamStruct,CDatabaseEx* pDB);
	//��ȡ������Ԫ
	HRESULT LoadExamRoomUnit(CXmlNode& _node,CGExamStruct* _pGExamStruct,CDatabaseEx* pDB);
	//����ʱ��
	HRESULT LoadExamTime(CXmlNode& _node,CGExamStruct* _pGExamStruct);
	//��ȡ��������
	HRESULT LoadExamListen(CXmlNode& _node,CGExamStruct* _pGExamStruct);
	//��
	HRESULT LoadExamAnswer(CXmlNode& _node,CGExamStruct* _pGExamStruct);

	//����ȫ�ֱ���������һ���࿼��Ϣ����
	HRESULT CreateExamInfoObj(CGExamStruct* _pGExamStruct,CDatabaseEx* _pDB);
	//��xml��ֻ����ʱ�䣬����load��ʱ��Ҫ���⴦��һ��
	HRESULT LoadTime(COleDateTime& _time,CComBSTR& _bstrVal);
private:
	CYDExamInfo*				m_pExamInfo;
	std::list<CYDStuMark*>		m_lstStuMark;//ѧ���ɼ�
	CString						m_strXmlFile;
};
