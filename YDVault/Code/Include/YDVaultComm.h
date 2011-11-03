#pragma once


//������ͼ��ǰ״̬

class CYDPropDef //���ԵĶ���
{
public:
	CYDPropDef(CString strPropName,UINT _type,CString _strDefault = _T(""))
	{
		m_strPropName = strPropName;
		m_uDataType = _type;
		m_defaultVal = _strDefault;  
	}
public:
	CString     m_strPropName;//������
	UINT        m_uDataType;//����
	CString     m_defaultVal;   //ȱʡֵ
	BOOL		m_bQuery;   //�ɲ�ѯ
};


class CQueryProp
{
public:
	CString m_strInternalName; 
	CString m_strDisplayName; 
	UINT    m_uType;
};

#define WM_YD_TREE_SEL_CHANGE       		WM_USER+1000   //������SelChange����Ϣ
#define WM_YD_OPEN_OBJECT                   WM_USER+1001   //��һ������
#define WM_YD_OPEN_DOCUMENT                 WM_USER+1002   
#define WM_YD_GET_PROPSHEETMANAGER          WM_USER+1003  //�õ�PropSheet������
#define WM_YD_CLOSE_ALL_PROP                WM_USER+1004  //�ر����е�PropSheet
#define	WM_YD_PFTREE_SEL_CHANGE				WM_USER+1005
#define WM_YD_EXAM_SEL_CHANGE				WM_USER+1006
#define WM_YD_OPEN_OBJECT_EX				WM_USER+1007
#define WM_YD_OPEN_DOCUMENT_EX				WM_USER+1008

#define WM_YD_EXAM_OPEN_DEPARTMENT          WM_USER+1009  //��Ժϵ��Ϣ
#define WM_YD_GET_FTPREF                    WM_USER+1010

#define WM_YD_SHOW_BELOWBAR                    WM_USER + 1011 //����Ϣ�Ƿ���ʾBELOWBAR
#define WM_YD_UPDATE_BELOWBAR                  WM_USER + 1012 //����Ϣ����BELOWBAR�İ�ť�Ƿ���Ч
#define WM_YD_ADD_PAGE                         WM_USER + 1013 //����Ϣ������������������ʾ��Ӧ�Ķ���
#define WM_YD_GET_DB						 WM_USER + 1014 //�õ����ݿ�Db
#define WM_YD_GET_BHICOLORICONS              WM_USER + 1015 //�õ��������Ƿ�ΪHiColorIcons
#define WM_YD_GET_EXAM_MSG_LISTBOX           WM_USER + 1016 //�õ�Msg��lstBox
#define WM_YD_GETLAST_SHEET                  WM_USER + 1017 //�õ�����򿪵�һ��Sheet
#define WM_YD_CLOSE_PROP					 WM_USER + 1018 //�ر�һ��ָ����Propҳ


#define WM_YD_OPEN_BASIC_VIEW	 WM_USER + 1019 //��ϰϵͳ�򿪻�������ҳ
#define WM_YD_UPDATE_PERSIONINFO	 WM_USER + 1020 //��ϰϵͳ�򿪻�������ҳ
#define WM_YD_UPDATE_HISTORYINFO	 WM_USER + 1021 //��ϰϵͳ�򿪻�������ҳ



//ȫ�ֹ���������Ϣ
#define WM_GET_MSG_DICTIONARY       WM_USER+1083    //�õ������ֵķ����ֵ�

#define WM_PROGRESSCTRL_SHOW        WM_USER+1084    //���ƽ�������ʾ
#define WM_PROGRESSCTRL_RANGE       WM_USER+1085    //��������Χ
#define WM_PROGRESSCTRL_SETIP       WM_USER+1086    //������ǰ��һ��
#define WM_PROGRESSCTRL_SHOWCANCEL  WM_USER+1087    //�������Ƿ�Ҫ��ʾȡ����ť

#define WM_YD_TEACHERCOM_BREAK		WM_USER+1088    //��ʦ����Com�˿ڶϿ���������USB�ߵ���

#define DB_QVAULTCLASSIFICATION             _T("QVAULTCLASSIFICATION")             //������
#define DB_QVAULTCLASSIFICATIONHIERARCHY    _T("QVCLASSIFICATIONHIERARCHY")			//�������ι�ϵ
#define DB_QUESTIONVAULT                    _T("QUESTIONVAULT")                    //���
#define DB_QCLASSIFICATIONQVAULT            _T("QCLASSIFICATIONQVAULT")            //������������ϵ
#define DB_QUESTIONTYPE                     _T("QUESTIONTYPE")                     //����
#define FIELD_QUESTIONTYPE_TYPE             _T("TYPE")
#define FIELD_QUESTIONTYPE_CREATOR          _T("CREATOR")
#define FIELD_QUESTIONTYPE_CREATEDATE       _T("CREATEDATE")
#define FIELD_QUESTIONTYPE_DESCRIPTION      _T("DESCRIPTION")

#define DB_QVAULTQTYPE                      _T("QVAULTQTYPE")                      //��������͹�ϵ
#define DB_KNOWLEDGEPOINT                   _T("KNOWLEDGEPOINT")                   //֪ʶ��
#define DB_QVAULTKPOINT                     _T("QVAULTKPOINT")                     //�����֪ʶ��
#define DB_CHOICEQUESTION                   _T("CHOICEQUESTION")                   //ѡ��������Ŀ
#define FIELD_CHOICEQUESTION_CODE			_T("CODE")
#define FIELD_CHOICEQUESTION_TITLE			_T("TITLE")
#define FIELD_CHOICEQUESTION_CHOICE			_T("CHOICE")
#define FIELD_CHOICEQUESTION_ANSWER			_T("ANSWER")
#define FIELD_CHOICEQUESTION_TYPEID			_T("TYPEID")
#define FIELD_CHOICEQUESTION_HARDLEVEL		_T("HARDLEVEL")
#define FIELD_CHOICEQUESTION_USEDCOUNT		_T("USEDCOUNT")
#define FIELD_CHOICEQUESTION_CREATOR		_T("CREATOR")
#define FIELD_CHOICEQUESTION_CREATEDATE		_T("CREATEDATE")
#define FIELD_CHOICEQUESTION_ANSWERDESC		_T("ANSWERDESC")
#define FIELD_CHOICEQUESTION_IMAGETITLE		_T("IMAGETITLE")
#define FIELD_CHOICEQUESTION_TITLEMODE		_T("TITLEMODE") // 0:�ı� 1:ͼ�� 2:���


#define DB_ARTICLEQUESTION                  _T("ARTICLEQUESTION")				//�����Ķ�������
#define FIELD_ARTICLEQUESTION_ARTICLE		_T("ARTICLE")
#define FIELD_ARTICLEQUESTION_TYPEID		_T("TYPEID")
#define FIELD_ARTICLEQUESTION_HARDLEVEL		_T("HARDLEVEL")
#define FIELD_ARTICLEQUESTION_USEDCOUNT		_T("USEDCOUNT")
#define FIELD_ARTICLEQUESTION_CREATOR		_T("CREATOR")
#define FIELD_ARTICLEQUESTION_ARTICLE		_T("ARTICLE")
#define FIELD_ARTICLEQUESTION_CREATEDATE	_T("CREATEDATE")
#define FIELD_ARTICLEQUESTION_QNUM			_T("QNUM")
#define FIELD_ARTICLEQUESTION_IMAGETITLE	_T("IMAGETITLE")
#define FIELD_ARTICLEQUESTION_TITLEMODE		_T("TITLEMODE") // 0:�ı� 1:ͼ�� 2:���



#define DB_ARTICLEQCHOICEQ                  _T("ARTICLEQCHOICEQ")                  //��ժ����Ŀ��ϵ
#define DB_VAULTQUESTION                    _T("QVAULTQUESTION")                    //���������Ĺ�ϵ
#define DB_QUESTIONKPOINT					_T("QUESTIONKPOINT")                  //������֪ʶ���ϵ
#define DB_PAPERFOLDER                      _T("PAPERFOLDER")                      //�Ծ��
#define DB_PFOLDERHIERARCHY                 _T("PFOLDERHIERARCHY")                 //�Ծ�в�ι�ϵ

#define DB_PAPER                            _T("PAPER")                            //�Ծ�
#define FIELD_YDPAPER_NAME                  _T("NAME")                             //�Ծ���
#define FIELD_YDPAPER_CREATOR               _T("CREATOR")                          //������
#define FIELD_YDPAPER_CREATEDATE            _T("CREATEDATE")                       //��������
#define FIELD_YDPAPER_DESCRIPTION           _T("DESCRIPTION")
#define FIELD_YDPAPER_CODE					_T("CODE")  //���
#define FIELD_YDPAPER_GUIDNAME              _T("GUIDNAME") //��
#define FIELD_YDPAPER_GUIDANSWERNAME		_T("GUIDANSWERNAME")//����

#define FIELD_YDVAULT_NAME                  _T("NAME")                             //�Ծ���


#define FIELD_YDPAPER_BEGIN1			_T("BEGIN1")  
#define FIELD_YDPAPER_END1              _T("END1") 
#define FIELD_YDPAPER_CHOICECOUNT1		_T("CHOICECOUNT1")
#define FIELD_YDPAPER_MULTIFLAG1		_T("MULTIFLAG1")
#define FIELD_YDPAPER_ANSWERCHOICE1		_T("ANSWERCHOICE1")

#define FIELD_YDPAPER_BEGIN2			_T("BEGIN2")  
#define FIELD_YDPAPER_END2              _T("END2") 
#define FIELD_YDPAPER_CHOICECOUNT2		_T("CHOICECOUNT2")
#define FIELD_YDPAPER_MULTIFLAG2		_T("MULTIFLAG2")
#define FIELD_YDPAPER_ANSWERCHOICE2		_T("ANSWERCHOICE2")

#define FIELD_YDPAPER_BEGIN3			_T("BEGIN3")  
#define FIELD_YDPAPER_END3              _T("END3") 
#define FIELD_YDPAPER_CHOICECOUNT3		_T("CHOICECOUNT3")
#define FIELD_YDPAPER_MULTIFLAG3		_T("MULTIFLAG3")
#define FIELD_YDPAPER_ANSWERCHOICE3		_T("ANSWERCHOICE3")


#define DB_PFOLDERPAPER                     _T("PFOLDERPAPER")                      //�Ծ�����Ծ��ϵ
#define DB_MEDIA					        _T("MEDIA")                            //ý��
#define DB_QUESTIONMEDIA					_T("QUESTIONMEDIA")                      //�����ý��Ĺ�ϵ

#define DB_ANSWER                           _T("STDANSWER")                            //��
#define FIELD_YDANSWER_NO					_T("ITEMNO")  //���
#define FIELD_YDANSWER_ANSWER                _T("ANSWER") //��
#define FIELD_YDANSWER_MARK					_T("MARK")//����

#define DB_VAULTLEVEL                       _T("VAULTLEVEL") //�����
#define FIELD_VAULTLEVEL_VAULTID					_T("VAULTID")  //���id
#define FIELD_VAULTLEVEL_LEVEL               _T("USERLEVEL") //�������
#define FIELD_VAULTLEVEL_CONDITION					_T("CONDITION")//�����������ȷ�ʣ�

#define DB_PAPERANSWER                     _T("PAPERANSWER")                      //�Ծ����׼�𰸹�ϵ

//��½�û�
#define DB_YDUSER					            _T("PERSON")							  //��¼�û���Ϣ
#define FIELD_YDUSER_NAME	     				_T("NAME")//�û���
#define FIELD_YDUSER_PASSWORD	     			_T("PASSWORD")//����
#define FIELD_YDUSER_AUTHORITY	     			_T("AUTHORITY")//Ȩ��ֵ
#define FIELD_YDUSER_GENDER	     			_T("GENDER")
#define FIELD_YDUSER_AGE	     			_T("AGE")
#define FIELD_YDUSER_ID	     				_T("ID")
#define FIELD_YDUSER_LEVEL	     			_T("STUDYLEVEL")
#define FIELD_YDUSER_DEPARTMENT	     		_T("DEPARTMENT")
#define FIELD_YDUSER_MPHONE		     		_T("MPHONE")
#define FIELD_YDUSER_QQ			     		_T("QQ")
#define FIELD_YDUSER_EMAIL			     		_T("EMAIL")

#define YD_NOT_FILL_ANSWER                      _T("δ��д")

//Ԥ���ֶΡ���ָ���ӳ���ϵ
#define DB_YDFACTORINFOITEM                     _T("FIELD2FACTOR")
#define FIELD_YDFACTORINFOITEM_VAULTID          _T("VAULTID")  //VaultID,����ID
#define FIELD_YDFACTORINFOITEM_QTYPEID          _T("QTYPEID")  //����ID
#define FIELD_YDFACTORINFOITEM_FACTORNAME       _T("FACTORNAME")  //ָ������
#define FIELD_YDFACTORINFOITEM_FIELDNAME        _T("FIELDNAME")  //Ԥ���ֶ�����
#define FIELD_YDFACTORINFOITEM_MIN				_T("MIN")        //��ֵ���½�
#define FIELD_YDFACTORINFOITEM_MAX              _T("MAX")  //��ֵ���Ͻ�
#define FIELD_YDFACTORINFOITEM_DESCRIPTION      _T("DESCRIPTION")  //����


enum QTYPE
{
	QTYPE_UNKNOWN = 0, //δ֪
	QTYPE_WRITING = 1 ,// ������
	QTYPE_LISTENINGCOMPREHENSION = 2,// ����
	QTYPE_READINGCOMPREHENSION = 3,//�Ķ����
	QTYPE_CLOZE = 4,//�������
	QTYPE_TRANSLATION = 5,//����
	QTYPE_VOCABULARY = 6,//ѡ����
	QTYPE_SKIMMING = 7,//�����Ķ�
	QTYPE_READINGVOCABULARY= 8,//�Ķ�ѡ�����
	QTYPE_LISTENINGVOCABULARY = 9,//�������
	QTYPE_LISTENINGSHORT = 10,//�̾�����
	QTYPE_LISTENINGLONG = 11,//��������
	QTYPE_ERRORCORRECTION = 12//��������
};

enum HARDLEVEL
{
	EASY = 1,
	HARD = 16
};

enum OPERATION
{
	OP_NEW = 1,
	OP_EDIT = 2,
	OP_DELETE = 3,
	OP_VIEW = 4
};

//ý������
enum MEDIATYPE
{
	M_IMAGE = 1,
	M_MP3 = 2,
	M_VEDIO = 3
};

//ý��ʹ��Ŀ��
enum MEDIAUSEFOR
{
	M_FORARTICLE = 1,//��������
	M_FORTITLE = 2,//��Ŀ
	M_FORANSWER = 3//��
};

//��ѯ������
enum QUERYOPERATE
{
	Q_UNKNOW = 0,
	Q_EQUAL = 1,//����
	Q_NOTEQUAL = 2,//������
	Q_CONTAIN = 3, //����
	Q_IN = 4,       //IN
	Q_NOTIN = 5,   //Not IN
	Q_GREATOR = 6,//����
	Q_LOWER = 7, //С��
	Q_GE = 8,//���ڵ���
	Q_LE=9//С�ڵ���
};

enum QUESTION_TYPE
{
	QUESTION_UNKNOWN = 0,   //δ����
	QUESTION_CHOICE = 1,    //DB_CHOICEQUESTION
	QUESTION_ARTICLE = 2    //DB_ARTICLEQUESTION
};

#define YD_DATE_FORMAT	  _T("%Y-%m-%d %H:%M:%S")

typedef UINT			OBJID;
const OBJID ID_EMPTY = 0;

const CString DEFAULT_CREATOR            =          _T("�Ͼ��Ұ۷�");

const UINT QUESTION_PAGE_COUNT = 1000;   //����ÿҳ��ʾ����
const WORD CRYPT_KEY           = 0x10 ;//�ӽ��ܵ�Keyֵ

enum SECURITY_TYPE
{
	SECURITY_VAULT			= 0,//��������
	SECURITY_PAPER			= 1,//�Ծ����
	SECURITY_OPERATE		= 2,//���Բ���
	SECURITY_BLUE_TOOTH     = 3,/*����*/
	SECURITY_REDOUT         = 4,/*����*/
	SECURITY_DATAMAINTAIN	= 5,//����ά��
	SECURITY_LOGIN			= 6	//��ϰϵͳ��¼�����
};

class COpenObjExParam //������ϢWM_YD_OPEN_OBJECT_EX�Ĳ����ṹ��
{
public:
	COpenObjExParam()
	{
		m_strName = _T("");
		m_Op = OP_VIEW;
		m_bUseStaicStruct = FALSE;
	}
	CString		m_strName;			//�ڵ������
	OPERATION	m_Op;				//�༭����������view��ͬ��ʽ
	BOOL		m_bUseStaicStruct;  //�ڳɼ���ѯ���Ծ���������ã�ΪFalseʱ��������
									//���ΪTrue��ʾҪ��ʾȫ�ֱ����е�ǰ�Ծ����Ϣ

};

const int cInfraRedRate = 2400;//����Ĳ�������2400
const int cBlueRedRate = 9600;//�����Ĳ�������9600

enum TITLEMODE
{
	TITLEMODE_TEXT,// 0:�ı�
	TITLEMODE_IMAGE,//1:ͼ��
	TITLEMODE_MIX  //2:���
};