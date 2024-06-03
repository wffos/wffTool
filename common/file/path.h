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

		//ȷ���ļ�����Ŀ¼����
		if (!PathFileExists(path))
			return true;

		WCHAR szPath[MAX_PATH] = { 0 };
		//Ŀ¼��·����2��\0��β
		swprintf_s(szPath, MAX_PATH, L"%s", path);

		SHFILEOPSTRUCT FileOp;
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
		FileOp.fFlags |= FOF_SILENT;        /*����ʾ����*/
		FileOp.fFlags |= FOF_NOERRORUI;        /*�����������Ϣ*/
		FileOp.fFlags |= FOF_NOCONFIRMATION;/*ֱ��ɾ����������ȷ��*/
		FileOp.fFlags |= FOF_ALLOWUNDO; /*�������վ*/
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.wFunc = FO_DELETE;
		FileOp.pFrom = szPath; /*Ҫɾ����Ŀ¼��������2��\0��β*/
		FileOp.pTo = NULL;

		/*ɾ��Ŀ¼*/
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
		GetEnvironmentVariable(_T("ALLUSERSPROFILE"), buff, MAX_PATH);		//��ȡ��������·��
		SHCreateDirectoryEx(NULL, szPath, NULL);		//�ݹ鴴��Ŀ¼

*/