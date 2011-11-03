#pragma once

#ifdef BASE_EXPORTS   
#define BASE_DLL_API __declspec(dllexport)   
#else   
#define BASE_DLL_API __declspec(dllimport)   
#endif   




//#define DB_ORACLE  //定义数据库的方式