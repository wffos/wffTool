#pragma once
#include <atlstr.h>
#include <ShlObj.h>

namespace wff
{
	CString GetExePath()
	{
		TCHAR szBuf[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szBuf, MAX_PATH);
		CString strExePath = szBuf;
		strExePath = strExePath.Left(strExePath.ReverseFind('\\') + 1);

		return strExePath;
	}

	CString GetExeName()
	{
		TCHAR szBuf[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szBuf, MAX_PATH);
		TCHAR szExt[_MAX_EXT] = { 0 };
		TCHAR szDir[_MAX_DIR] = { 0 };
		TCHAR szDrive[_MAX_DRIVE] = { 0 };
		TCHAR szExeName[_MAX_FNAME] = { 0 };
		_wsplitpath_s(szBuf, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szExeName, _MAX_FNAME, szExt, _MAX_EXT);
		CString strExeName = szExeName;

		return strExeName;
	}

	HMODULE GetSelfModuleHandle()
	{
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
	}

	CString GetModulePath()
	{
		TCHAR szBuf[MAX_PATH] = { 0 };
		GetModuleFileName(GetSelfModuleHandle(), szBuf, MAX_PATH);
		CString strExePath = szBuf;
		strExePath = strExePath.Left(strExePath.ReverseFind('\\') + 1);

		return strExePath;
	}

	bool RemovePath(const CString& path)
	{
		if (path.IsEmpty() || path.GetLength() > MAX_PATH)
		{
			return false;
		}

		//确保文件或者目录存在
		if (!PathFileExists(path))
			return true;

		WCHAR szPath[MAX_PATH] = { 0 };
		//目录的路径以2个\0结尾
		swprintf_s(szPath, MAX_PATH, L"%s", path);

		SHFILEOPSTRUCT FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
		FileOp.fFlags |= FOF_SILENT;        /*不显示进度*/
		FileOp.fFlags |= FOF_NOERRORUI;        /*不报告错误信息*/
		FileOp.fFlags |= FOF_NOCONFIRMATION;/*直接删除，不进行确认*/
		FileOp.fFlags |= FOF_ALLOWUNDO; /*放入回收站*/
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.wFunc = FO_DELETE;
		FileOp.pFrom = szPath; /*要删除的目录，必须以2个\0结尾*/
		FileOp.pTo = NULL;

		/*删除目录*/
		if (0 == SHFileOperation(&FileOp))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

/*
		GetEnvironmentVariable(_T("ALLUSERSPROFILE"), buff, MAX_PATH);		//获取环境变量路径
		SHCreateDirectoryEx(NULL, szPath, NULL);		//递归创建目录

*/