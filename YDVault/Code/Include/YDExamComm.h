#pragma once

//考试操作
#define EXAM_SERACH_TEACHER				_T("搜索教师机")
#define EXAM_FM_REDIO                   _T("FM收音机")
#define EXAM_BULETOOTH_RECEIVE_PAPER	_T("蓝牙收卷")
#define EXAM_REDOUT_RECEIVE_PAPER		_T("红外收卷")
#define EXAM_USB_RECEIVE_PAPER			_T("USB收卷")
#define EXAM_EXAM_BACK					_T("考试备份")
#define EXAM_SPECIAL_DELAY				_T("迟到")
#define EXAM_SPECIAL_BEFORE				_T("提前交卷")
#define EXAM_SPECIAL_EQUIP_REPLCACE		_T("设备更换")
#define EXAM_INFO_INPUT					_T("信息导入")
#define EXAM_CREATE_PAPER_FILE			_T("生成考试文件")
#define EXAM_PIC_GATHER					_T("图像采集")
#define EXAM_PAPER_READOVER             _T("试卷批阅")
#define EXAM_EXAM_OPERATE               _T("考试操作")
#define EXAM_EXAM                       _T("考试")
#define EXAM_MARK_INFO					_T("成绩查询")
#define EXAM_PAPER_ANSYS				_T("试卷分析")
#define EXAM_ERR_REPORT					_T("错题报表")
#define EXAN_FETCH_ANSWER               _T("红外提取答案")
#define EXAM_EXTACT_ANSWER              _T("教师机提取答案")
#define EXAM_INFRARED                   _T("红外考试")
#define EXAM_BLUE						_T("蓝牙考试")

//数据管理
#define EXAM_DATAMGR_TREE				_T("数据维护")
#define EXAM_DEPT_INFO					_T("院系信息")
#define EXAM_STU_INFO					_T("学生信息")
#define EXAM_STU_DEVICE					_T("学生设备")
#define EXAM_ANSWER_INFO                _T("答案信息")
#define EXAM_TEACHER_DEVICE_INFO		_T("教师设备")
#define EXAM_PAPER_INFO					_T("试卷设置")
#define EXAM_SUBJECT_INFO				_T("考试科目")
#define EXAM_EXAMADDR_INFO              _T("考场信息")


#define DB_YDSTUINFO                        _T("STUDENT")   //学生信息表
#define FIELD_YDSTU_ID						_T("ID")       //学号
#define FIELD_YDSTU_NAME                    _T("NAME")     //学生姓名
#define FIELD_YDSTU_GENDER					_T("GENDER")   //学生性别
#define FIELD_YDSTU_AGE						_T("AGE")	  //学生年龄
#define FIELD_YDSTU_TEL						_T("TEL")	  //学生电话
#define FIELD_YDSTU_MAIL					_T("EMAIL")	  //学生Emai
#define FIELD_YDSTU_PHOTO					_T("PHOTO")	  //学生图片
#define FIELD_YDSTU_EVID					_T("DEVICEID")	  //设备ID



#define DB_YDDEPARTMENT                     _T("DEPARTMENT") //部门信息
#define DB_YDLINK_DEPARTMENT_CLASS          _T("DEPTCLS") //部门与班级的关系

#define FIELD_YDDEPARTMENT_ID               _T("ID")     //部门ID
#define FIELD_YDDEPARTMENT_NAME             _T("NAME")   //部门名称
#define FIELD_YDDEPARTMENT_PRESIDENT        _T("PRESIDENT")    //院长
#define FIELD_YDDEPARTMENT_PRESIDENT_TEL    _T("TEL") //联系方式

#define DB_YDCLASS                          _T("CLASS")   //班级
#define DB_YDLINK_CLASS_STUDENT             _T("CLSSTU") //班级与学生的关系

#define FIELD_YDCLASS_CLASSID               _T("ID")  //班级ID
#define FIELD_YDCLASS_NAME                  _T("NAME") //班级名称
#define FIELD_YDCLASS_HEAD					_T("HEAD")//	班主任
#define FIELD_YDCLASS_HEADTEL               _T("TEL") //	联系方式

#define DB_YDEXAMADDR                        _T("EXAMADDR")   //考场
#define DB_YDLINK_EXAMADDR_UNIT          _T("EADDRUNIT") //考场与考场单元

#define FIELD_YDEXAMADDR_ID						_T("ID")  //考场ID
#define FIELD_YDEXAMADDR_ADDR                  _T("ADDR") //考场地点

#define DB_YDTEACHERDEV		                _T("TEACHERDEVICE") //教师机地址信息
#define FIELD_YDTEACHERDEV_TEACHID          _T("ID")   //	教师编号
#define FIELD_YDTEACHERDEV_NAME             _T("NAME") //	教师姓名
#define FIELD_YDTEACHERDEV_DEVID			_T("DEVICEID")//教师机设备号

#define DB_YDSUBJECT                        _T("SUBJECT")  //考试科目信息表
#define FIELD_YDSUBJECT_ID                  _T("ID")	 //科目编号
#define FIELD_YDSUBJECT_NAME                 _T("NAME")	//科目的名称

#define DB_YDLINK_SUBJECT_PAPER             _T("SUBJECTPAPER") //考试科目与试卷的关系

#define DB_YDEXAMADDRUNIT                   _T("EXAMADDRUNIT")  
#define FIELD_YDEXAMADDRUNIT_STUID          _T("STUID")	
#define FIELD_YDEXAMADDRUNIT_DEVICEID       _T("DEVICEID")	
#define FIELD_YDEXAMADDRUNIT_EXAMID			_T("EXAMID")

#define DB_YDSTUMARK                   _T("STUMARK")  
#define FIELD_YDSTUMARK_STUID          _T("STUID")	
#define FIELD_YDSTUMARK_DEVICEID       _T("DEVICEID")	
#define FIELD_YDSTUMARK_EXAMINFOID     _T("EXAMINFOID")	
#define FIELD_YDSTUMARK_EXAMID			_T("EXAMID")
#define FIELD_YDSTUMARK_ANSWER			_T("ANSWER")
#define FIELD_YDSTUMARK_PAPERID			_T("PAPERID")
#define FIELD_YDSTUMARK_MARK			_T("MARK")
#define FIELD_YDSTUMARK_SUBJECTID		_T("SUBJECTID")
#define FIELD_YDSTUMARK_EXAMDATE		_T("EXAMDATE")

#define DB_YDRESULT                                 _T("ydresult")   //学生答案信息表
#define FIELD_YDRESULT_STUDID						_T("StudID")      //学生信息表OBJID
#define FIELD_YDRESULT_PAPERID                      _T("PaperID")     //试卷编号
#define FIELD_YDRESULT_START						_T("Studstart")	  //开考时间
#define FIELD_YDRESULT_END							_T("Studend")	  //交卷时间
#define FIELD_YDRESULT_ASNSWER						_T("Studanswer")  //学生答案
#define FIELD_YDRESULT_ADDRID                       _T("AddrID")	  //考场编号

#define DB_YDDEVICE							_T("DEVICE")   //设备表

#define FIELD_YDDEVICE_ID					_T("ID")  //设备编号
#define FIELD_YDDEVICE_MAC					_T("MAC") //设备蓝牙地址

#define DB_YDEXAM_INFO						_T("EXAMINFO")  //监考信息
#define FIELD_YEEXAMINFO_PAPERID            _T("PAPERID")   //试卷OBJID
#define FIELD_YEEXAMINFO_STARTTIME          _T("StartTime")   //开始时间
#define FIELD_YEEXAMINFO_ENDTIME            _T("EndTime")   //结束时间
#define FIELD_YEEXAMINFO_TEACHER            _T("TEACHER")   //监考老师
#define FIELD_YEEXAMINFO_EXAMXML            _T("EXAMXML")   //考试信息文件名

#define DB_YDLINK_EADDREINFO                _T("EADDREINFO") //考场与考试信息关系

#define IDC_EXAM_DM_QUERY_STU_CONN		100000

enum YDEXAM_ROLLCALL_TYPE //点名方式
{
	ROLLCALL_TYPE_UNKNOW = -1, //未知
	ROLLCALL_TYPE_BLUE = 0,       //蓝牙
	ROLLCALL_TYPE_INFRAR = 1,     //红外
	ROLLCALL_TYPE_USB = 2          //USB
};

enum YDEXAM_ROLLING_TYPE //收卷方式
{
	ROLLING_TYPE_UNKNOW = -1, //未知
	ROLLING_TYPE_BLUE = 0,       //蓝牙
	ROLLING_TYPE_INFRAR = 1,     //红外
	ROLLING_TYPE_USB = 2          //USB
};

enum YDEXAM_KIND //考试性质
{
	YDEXAM_KIND_NORMAL = 0,//正式考试
	YDEXAM_KIND_TEST  = 1 //一般测试
};

enum YDEAM_STEP //考试的步骤
{
	YDEXAM_STEP_NOT = -1,//什么也没有进入
	YDEXAM_STEP_ROOLCALL = 0,//已经点名过
	YDEXAM_STEP_BGGINEXAM = 1,//已经开考过
	YDEXAM_STEP_ROLLING  = 2//已经交卷
};

