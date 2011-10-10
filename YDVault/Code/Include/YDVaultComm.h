#pragma once


//对象视图当前状态

class CYDPropDef //属性的定义
{
public:
	CYDPropDef(CString strPropName,UINT _type,CString _strDefault = _T(""))
	{
		m_strPropName = strPropName;
		m_uDataType = _type;
		m_defaultVal = _strDefault;  
	}
public:
	CString     m_strPropName;//属性名
	UINT        m_uDataType;//类型
	CString     m_defaultVal;   //缺省值
	BOOL		m_bQuery;   //可查询
};


class CQueryProp
{
public:
	CString m_strInternalName; 
	CString m_strDisplayName; 
	UINT    m_uType;
};

#define WM_YD_TREE_SEL_CHANGE       		WM_USER+1000   //在树上SelChange的消息
#define WM_YD_OPEN_OBJECT                   WM_USER+1001   //打开一个对象
#define WM_YD_OPEN_DOCUMENT                 WM_USER+1002   
#define WM_YD_GET_PROPSHEETMANAGER          WM_USER+1003  //得到PropSheet管理器
#define WM_YD_CLOSE_ALL_PROP                WM_USER+1004  //关闭所有的PropSheet
#define	WM_YD_PFTREE_SEL_CHANGE				WM_USER+1005
#define WM_YD_EXAM_SEL_CHANGE				WM_USER+1006
#define WM_YD_OPEN_OBJECT_EX				WM_USER+1007
#define WM_YD_OPEN_DOCUMENT_EX				WM_USER+1008

#define WM_YD_EXAM_OPEN_DEPARTMENT          WM_USER+1009  //打开院系信息
#define WM_YD_GET_FTPREF                    WM_USER+1010

#define WM_YD_SHOW_BELOWBAR                    WM_USER + 1011 //发消息是否显示BELOWBAR
#define WM_YD_UPDATE_BELOWBAR                  WM_USER + 1012 //发消息控制BELOWBAR的按钮是否有效
#define WM_YD_ADD_PAGE                         WM_USER + 1013 //发消息，由主窗口来控制显示对应的对象
#define WM_YD_GET_DB						 WM_USER + 1014 //得到数据库Db
#define WM_YD_GET_BHICOLORICONS              WM_USER + 1015 //得到主窗口是否为HiColorIcons
#define WM_YD_GET_EXAM_MSG_LISTBOX           WM_USER + 1016 //得到Msg的lstBox
#define WM_YD_GETLAST_SHEET                  WM_USER + 1017 //得到最近打开的一个Sheet
#define WM_YD_CLOSE_PROP					 WM_USER + 1018 //关闭一个指定的Prop页


#define WM_YD_OPEN_BASIC_VIEW	 WM_USER + 1019 //练习系统打开基本属性页
#define WM_YD_UPDATE_PERSIONINFO	 WM_USER + 1020 //练习系统打开基本属性页
#define WM_YD_UPDATE_HISTORYINFO	 WM_USER + 1021 //练习系统打开基本属性页



//全局管理器的消息
#define WM_GET_MSG_DICTIONARY       WM_USER+1083    //得到多语种的翻译字典

#define WM_PROGRESSCTRL_SHOW        WM_USER+1084    //控制进度条显示
#define WM_PROGRESSCTRL_RANGE       WM_USER+1085    //进度条范围
#define WM_PROGRESSCTRL_SETIP       WM_USER+1086    //进度条前进一格
#define WM_PROGRESSCTRL_SHOWCANCEL  WM_USER+1087    //进度条是否要显示取消按钮

#define WM_YD_TEACHERCOM_BREAK		WM_USER+1088    //教师机从Com端口断开，可能是USB线掉了

#define DB_QVAULTCLASSIFICATION             _T("QVAULTCLASSIFICATION")             //题库大类
#define DB_QVAULTCLASSIFICATIONHIERARCHY    _T("QVCLASSIFICATIONHIERARCHY")			//题库大类层次关系
#define DB_QUESTIONVAULT                    _T("QUESTIONVAULT")                    //题库
#define DB_QCLASSIFICATIONQVAULT            _T("QCLASSIFICATIONQVAULT")            //题库大类与题库关系
#define DB_QUESTIONTYPE                     _T("QUESTIONTYPE")                     //题型
#define FIELD_QUESTIONTYPE_TYPE             _T("TYPE")
#define FIELD_QUESTIONTYPE_CREATOR          _T("CREATOR")
#define FIELD_QUESTIONTYPE_CREATEDATE       _T("CREATEDATE")
#define FIELD_QUESTIONTYPE_DESCRIPTION      _T("DESCRIPTION")

#define DB_QVAULTQTYPE                      _T("QVAULTQTYPE")                      //题库与题型关系
#define DB_KNOWLEDGEPOINT                   _T("KNOWLEDGEPOINT")                   //知识点
#define DB_QVAULTKPOINT                     _T("QVAULTKPOINT")                     //题库与知识点
#define DB_CHOICEQUESTION                   _T("CHOICEQUESTION")                   //选择类型题目
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
#define FIELD_CHOICEQUESTION_TITLEMODE		_T("TITLEMODE") // 0:文本 1:图像 2:混合


#define DB_ARTICLEQUESTION                  _T("ARTICLEQUESTION")				//带有阅读资料题
#define FIELD_ARTICLEQUESTION_ARTICLE		_T("ARTICLE")
#define FIELD_ARTICLEQUESTION_TYPEID		_T("TYPEID")
#define FIELD_ARTICLEQUESTION_HARDLEVEL		_T("HARDLEVEL")
#define FIELD_ARTICLEQUESTION_USEDCOUNT		_T("USEDCOUNT")
#define FIELD_ARTICLEQUESTION_CREATOR		_T("CREATOR")
#define FIELD_ARTICLEQUESTION_ARTICLE		_T("ARTICLE")
#define FIELD_ARTICLEQUESTION_CREATEDATE	_T("CREATEDATE")
#define FIELD_ARTICLEQUESTION_QNUM			_T("QNUM")
#define FIELD_ARTICLEQUESTION_IMAGETITLE	_T("IMAGETITLE")
#define FIELD_ARTICLEQUESTION_TITLEMODE		_T("TITLEMODE") // 0:文本 1:图像 2:混合



#define DB_ARTICLEQCHOICEQ                  _T("ARTICLEQCHOICEQ")                  //文摘与题目关系
#define DB_VAULTQUESTION                    _T("QVAULTQUESTION")                    //题库与试题的关系
#define DB_QUESTIONKPOINT					_T("QUESTIONKPOINT")                  //问题与知识点关系
#define DB_PAPERFOLDER                      _T("PAPERFOLDER")                      //试卷夹
#define DB_PFOLDERHIERARCHY                 _T("PFOLDERHIERARCHY")                 //试卷夹层次关系

#define DB_PAPER                            _T("PAPER")                            //试卷
#define FIELD_YDPAPER_NAME                  _T("NAME")                             //试卷名
#define FIELD_YDPAPER_CREATOR               _T("CREATOR")                          //创建人
#define FIELD_YDPAPER_CREATEDATE            _T("CREATEDATE")                       //创建日期
#define FIELD_YDPAPER_DESCRIPTION           _T("DESCRIPTION")
#define FIELD_YDPAPER_CODE					_T("CODE")  //题号
#define FIELD_YDPAPER_GUIDNAME              _T("GUIDNAME") //答案
#define FIELD_YDPAPER_GUIDANSWERNAME		_T("GUIDANSWERNAME")//分数

#define FIELD_YDVAULT_NAME                  _T("NAME")                             //试卷名


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


#define DB_PFOLDERPAPER                     _T("PFOLDERPAPER")                      //试卷夹与试卷关系
#define DB_MEDIA					        _T("MEDIA")                            //媒体
#define DB_QUESTIONMEDIA					_T("QUESTIONMEDIA")                      //试题和媒体的关系

#define DB_ANSWER                           _T("STDANSWER")                            //答案
#define FIELD_YDANSWER_NO					_T("ITEMNO")  //题号
#define FIELD_YDANSWER_ANSWER                _T("ANSWER") //答案
#define FIELD_YDANSWER_MARK					_T("MARK")//分数

#define DB_VAULTLEVEL                       _T("VAULTLEVEL") //题库层次
#define FIELD_VAULTLEVEL_VAULTID					_T("VAULTID")  //题库id
#define FIELD_VAULTLEVEL_LEVEL               _T("USERLEVEL") //层次描述
#define FIELD_VAULTLEVEL_CONDITION					_T("CONDITION")//层次条件（正确率）

#define DB_PAPERANSWER                     _T("PAPERANSWER")                      //试卷与标准答案关系

//登陆用户
#define DB_YDUSER					            _T("PERSON")							  //登录用户信息
#define FIELD_YDUSER_NAME	     				_T("NAME")//用户名
#define FIELD_YDUSER_PASSWORD	     			_T("PASSWORD")//密码
#define FIELD_YDUSER_AUTHORITY	     			_T("AUTHORITY")//权限值
#define FIELD_YDUSER_GENDER	     			_T("GENDER")
#define FIELD_YDUSER_AGE	     			_T("AGE")
#define FIELD_YDUSER_ID	     				_T("ID")
#define FIELD_YDUSER_LEVEL	     			_T("STUDYLEVEL")
#define FIELD_YDUSER_DEPARTMENT	     		_T("DEPARTMENT")
#define FIELD_YDUSER_MPHONE		     		_T("MPHONE")
#define FIELD_YDUSER_QQ			     		_T("QQ")
#define FIELD_YDUSER_EMAIL			     		_T("EMAIL")

#define YD_NOT_FILL_ANSWER                      _T("未填写")

//预留字段”与指标的映射关系
#define DB_YDFACTORINFOITEM                     _T("FIELD2FACTOR")
#define FIELD_YDFACTORINFOITEM_VAULTID          _T("VAULTID")  //VaultID,题库的ID
#define FIELD_YDFACTORINFOITEM_QTYPEID          _T("QTYPEID")  //题型ID
#define FIELD_YDFACTORINFOITEM_FACTORNAME       _T("FACTORNAME")  //指标名称
#define FIELD_YDFACTORINFOITEM_FIELDNAME        _T("FIELDNAME")  //预留字段名称
#define FIELD_YDFACTORINFOITEM_MIN				_T("MIN")        //数值型下界
#define FIELD_YDFACTORINFOITEM_MAX              _T("MAX")  //数值型上界
#define FIELD_YDFACTORINFOITEM_DESCRIPTION      _T("DESCRIPTION")  //描述


enum QTYPE
{
	QTYPE_UNKNOWN = 0, //未知
	QTYPE_WRITING = 1 ,// 作文题
	QTYPE_LISTENINGCOMPREHENSION = 2,// 听力
	QTYPE_READINGCOMPREHENSION = 3,//阅读理解
	QTYPE_CLOZE = 4,//完形填空
	QTYPE_TRANSLATION = 5,//翻译
	QTYPE_VOCABULARY = 6,//选择题
	QTYPE_SKIMMING = 7,//快速阅读
	QTYPE_READINGVOCABULARY= 8,//阅读选词填空
	QTYPE_LISTENINGVOCABULARY = 9,//听力填空
	QTYPE_LISTENINGSHORT = 10,//短句听力
	QTYPE_LISTENINGLONG = 11,//长句听力
	QTYPE_ERRORCORRECTION = 12//长句听力
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

//媒体类型
enum MEDIATYPE
{
	M_IMAGE = 1,
	M_MP3 = 2,
	M_VEDIO = 3
};

//媒体使用目的
enum MEDIAUSEFOR
{
	M_FORARTICLE = 1,//文章配套
	M_FORTITLE = 2,//题目
	M_FORANSWER = 3//答案
};

//查询操作符
enum QUERYOPERATE
{
	Q_UNKNOW = 0,
	Q_EQUAL = 1,//等于
	Q_NOTEQUAL = 2,//不等于
	Q_CONTAIN = 3, //包含
	Q_IN = 4,       //IN
	Q_NOTIN = 5,   //Not IN
	Q_GREATOR = 6,//大于
	Q_LOWER = 7, //小于
	Q_GE = 8,//大于等于
	Q_LE=9//小于等于
};

enum QUESTION_TYPE
{
	QUESTION_UNKNOWN = 0,   //未定义
	QUESTION_CHOICE = 1,    //DB_CHOICEQUESTION
	QUESTION_ARTICLE = 2    //DB_ARTICLEQUESTION
};

#define YD_DATE_FORMAT	  _T("%Y-%m-%d %H:%M:%S")

typedef UINT			OBJID;
const OBJID ID_EMPTY = 0;

const CString DEFAULT_CREATOR            =          _T("南京我佰分");

const UINT QUESTION_PAGE_COUNT = 1000;   //试题每页显示个数
const WORD CRYPT_KEY           = 0x10 ;//加解密的Key值

enum SECURITY_TYPE
{
	SECURITY_VAULT			= 0,//试题库管理
	SECURITY_PAPER			= 1,//试卷管理
	SECURITY_OPERATE		= 2,//考试操作
	SECURITY_BLUE_TOOTH     = 3,/*蓝牙*/
	SECURITY_REDOUT         = 4,/*红外*/
	SECURITY_DATAMAINTAIN	= 5,//数据维护
	SECURITY_LOGIN			= 6	//练习系统登录试题库
};

class COpenObjExParam //发送消息WM_YD_OPEN_OBJECT_EX的参数结构体
{
public:
	COpenObjExParam()
	{
		m_strName = _T("");
		m_Op = OP_VIEW;
		m_bUseStaicStruct = FALSE;
	}
	CString		m_strName;			//节点的名字
	OPERATION	m_Op;				//编辑，新增还是view不同方式
	BOOL		m_bUseStaicStruct;  //在成绩查询和试卷分析是有用，为False时不做处理
									//如果为True表示要显示全局变量中当前试卷的信息

};

const int cInfraRedRate = 2400;//红外的波特率是2400
const int cBlueRedRate = 9600;//蓝牙的波特率是9600

enum TITLEMODE
{
	TITLEMODE_TEXT,// 0:文本
	TITLEMODE_IMAGE,//1:图像
	TITLEMODE_MIX  //2:混合
};