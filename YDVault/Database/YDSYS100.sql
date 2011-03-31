--用户
CREATE TABLE "PERSION" 
 (	
"OBJID" NUMBER(38,0) NOT NULL ENABLE, 
"NAME" NVARCHAR2(128 ) NOT NULL ENABLE, 
"PASSWORD" NVARCHAR2(64),
"AUTHORITY" NUMBER(38,0) NOT NULL ENABLE,
 CONSTRAINT "PERSION_PK" PRIMARY KEY ("OBJID")
  );
create sequence S_PERSION
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--部门
CREATE TABLE "DEPARTMENT" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE, 
"NAME" NVARCHAR2(128) NOT NULL ENABLE, 
"PRESIDENT" NVARCHAR2(128), 
"TEL" NVARCHAR2(16), 
CONSTRAINT "DEPARTMENT_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_DEPARTMENT
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--班级
CREATE TABLE "CLASS" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE, 
"NAME" NVARCHAR2(128) NOT NULL ENABLE, 
"HEAD" NVARCHAR2(128), 
"TEL" NVARCHAR2(16), 
CONSTRAINT "CLASS_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_CLASS
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--部门与班级
CREATE TABLE "DEPTCLS" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "DEPTCLS_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_DEPTCLS
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--学生
CREATE TABLE "STUDENT" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE, 
"NAME" NVARCHAR2(128) NOT NULL ENABLE, 
"GENDER" NUMBER, 
"AGE" NUMBER, 
"TEL" NVARCHAR2(16), 
"EMAIL" NVARCHAR2(128), 
"DEVICEID" NVARCHAR2(64),
"PHOTO" BLOB,  
CONSTRAINT "STUDENT_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_STUDENT
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--部门与学生
CREATE TABLE "DEPTSTU" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "DEPTSTU_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_DEPTSTU
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;
--班级与学生
CREATE TABLE "CLSSTU" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "CLSSTU_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_CLSSTU
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--教师机信息
CREATE TABLE "TEACHERDEVICE" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE, 
"NAME" NVARCHAR2(128) NOT NULL ENABLE, 
"DEVICEID" NVARCHAR2(64),
CONSTRAINT "TEACHERDEVICE_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_TEACHERDEVICE
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--考场信息
CREATE TABLE "EXAMADDR" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE, 
"ADDR" NVARCHAR2(128) NOT NULL ENABLE, 
CONSTRAINT "EXAMADDR_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_EXAMADDR
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;
--考场与学生
CREATE TABLE "EXAMADDRSTU" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "EXAMADDRSTU_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_EXAMADDRSTU
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--考试科目
CREATE TABLE "SUBJECT" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE, 
"NAME" NVARCHAR2(128) NOT NULL ENABLE, 
CONSTRAINT "SUBJECT_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_SUBJECT
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--考试科目与试卷
CREATE TABLE "SUBJECTPAPER" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "SUBJECTPAPER_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_SUBJECTPAPER
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;


--标准答案
CREATE TABLE "STDANSWER" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"NO" NUMBER NOT NULL ENABLE, 
"ANSWER" NVARCHAR2(8) NOT NULL ENABLE, 
"MARK" NUMBER, 
CONSTRAINT "STDANSWER_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_STDANSWER
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--考场单元
CREATE TABLE "EXAMADDRUNIT" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"STUID" NUMBER NOT NULL ENABLE, 
"DEVICE" NUMBER NOT NULL ENABLE, 
"EXAMID" NVARCHAR2(32) NOT NULL ENABLE, 
CONSTRAINT "EXAMADDRUNIT_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_EXAMADDRUNIT
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--学生分数
CREATE TABLE "STUMARK" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"SUTID" NUMBER NOT NULL ENABLE, 
"DEVICEID" NUMBER NOT NULL ENABLE, 
"EXAMINFOID" NUMBER NOT NULL ENABLE, 
"PAPERID" NUMBER NOT NULL ENABLE, 
"EXAMID" NVARCHAR2(32) NOT NULL ENABLE, 
"ANSWER" NVARCHAR2(500) NOT NULL ENABLE, 
"MARK" NVARCHAR2(500) NOT NULL ENABLE, 
CONSTRAINT "STUMARK_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_STUMARK
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--考试信息
CREATE TABLE "EXAMINFO" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"PAPERID" NUMBER NOT NULL ENABLE,
"STARTTIME" DATE,
"ENDTIME" DATE,
"TEACHER" VARCHAR2(32 BYTE),
CONSTRAINT "EXAMINFO_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_EXAMINFO
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;
--设备信息
CREATE TABLE "DEVICE" 
 (
"OBJID" NUMBER NOT NULL ENABLE, 
"ID" NVARCHAR2(32) NOT NULL ENABLE,
"MAC" NVARCHAR2(8) NOT NULL ENABLE,

CONSTRAINT "DEVICE_PK" PRIMARY KEY ("OBJID")
 );
 
create sequence S_DEVICE
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;
--考场与考场单元
CREATE TABLE "EADDRUNIT" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "EADDRUNIT_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_EADDRUNIT
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;
--考场与考试信息
CREATE TABLE "EADDREINFO" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "EADDREINFO_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_EADDREINFO
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;
--试卷与答案
CREATE TABLE "PAPERANSWER" 
 (	
"OBJID" NUMBER NOT NULL ENABLE, 
"ID_A" NUMBER NOT NULL ENABLE, 
"ID_B" NUMBER NOT NULL ENABLE, 
 CONSTRAINT "PAPERANSWER_PK" PRIMARY KEY ("OBJID")
  );
  
create sequence S_PAPERANSWER
increment by 1 
start with 1
nomaxvalue
order
nocycle
nocache;

--触发器
create or replace trigger tg_DEPTSTU
BEFORE INSERT ON DEPTSTU
FOR EACH ROW
BEGIN
  SELECT S_DEPTSTU.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/

create or replace trigger tg_DEPTCLS
BEFORE INSERT ON DEPTCLS
FOR EACH ROW
BEGIN
  SELECT S_DEPTCLS.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/

create or replace trigger tg_CLSSTU
BEFORE INSERT ON CLSSTU
FOR EACH ROW
BEGIN
  SELECT S_CLSSTU.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/

create or replace trigger tg_EXAMADDRSTU
BEFORE INSERT ON EXAMADDRSTU
FOR EACH ROW
BEGIN
  SELECT S_EXAMADDRSTU.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/
create or replace trigger tg_PAPERANSWER
BEFORE INSERT ON PAPERANSWER
FOR EACH ROW
BEGIN
  SELECT S_PAPERANSWER.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/
create or replace trigger tg_EADDRUNIT
BEFORE INSERT ON EADDRUNIT
FOR EACH ROW
BEGIN
  SELECT S_EADDRUNIT.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/
create or replace trigger tg_EADDREINFO
BEFORE INSERT ON EADDREINFO
FOR EACH ROW
BEGIN
  SELECT S_EADDREINFO.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/
create or replace trigger tg_SUBJECTPAPER
BEFORE INSERT ON SUBJECTPAPER
FOR EACH ROW
BEGIN
  SELECT S_SUBJECTPAPER.NEXTVAL INTO :NEW.OBJID FROM dual;
END;
/

