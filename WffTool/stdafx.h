// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once
/***********************************************/
//soui头文件
#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>
#include "resource.h"
#define R_IN_CPP	//定义这个开关来
#include "res\resource.h"
using namespace SOUI;
/***********************************************/


/***********************************************/
//c/c++ 标准头文件
#include <vector>
#include <map>
#include<list>
using namespace std;
/***********************************************/


/***********************************************/
//Windows头文件
#include <atlstr.h>
#include <atltime.h>
#include <ShlObj.h>
#include <ShellAPI.h>
/***********************************************/


/***********************************************/
//工程头文件
#include "include/CommonFunc.h"
/***********************************************/

#include "../common/database/WffDBManager.h"
#include "../common/file/FindFileInDir_win.h"

#define WM_CDG_BASE							(WM_USER+20000)
#define WM_SHOWMASK							WM_CDG_BASE+1		//显示/隐藏mask
#define WM_REFRESH_WINDOW				WM_CDG_BASE+2		//刷新界面

#define MAX_KEY_LEN 32

struct FileInfo 
{
	FileInfo()
	{
		strName = L"";
		strFullPath = L"";
		nFileType = 0;
		nEncType = 0;
		nLength = 0;
		editTime = L"";
	}

	int nFileType;					//类型（0磁盘 1文件夹 2文件）
	int nEncType;					//0:明文 1:密文
	ULONGLONG nLength;		//文件大小
	CString strName;				//文件名
	CString strFullPath;			//全路径
	CString editTime;				//修改时间
	CString strExtType;			//后缀类型
};

extern BOOL g_bBackup;

extern void FindFileInDir(CString strDir, list<FileInfo>& listFile);
