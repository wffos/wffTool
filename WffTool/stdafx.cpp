// stdafx.cpp : source file that includes just the standard includes
//	SkinTest.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

BOOL g_bBackup = FALSE;

void FindFileInDir(CString strDir, list<FileInfo>& listFile)			//
{
	//CKWow64FsRedrt tmpWow64;
	//tmpWow64.Close();		//关闭重定向

	CString strSrcPath = strDir;
	TCHAR szFind[MAX_PATH] = {0};
	_tcscpy_s(szFind, MAX_PATH, (strDir + L"\\*.*"));
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE != hFind)
	{
		FileInfo fileInfo;
		FILETIME ftLocal;
		SYSTEMTIME stTime;

		do
		{
			fileInfo.strName = L"";
			fileInfo.strFullPath = L"";
			fileInfo.nFileType = 0;
			fileInfo.nLength = 0;
			fileInfo.editTime = L"";

			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}
			fileInfo.strName = FindFileData.cFileName;
			fileInfo.strFullPath = strDir + L"\\" + FindFileData.cFileName;

			if( !(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
			{
				fileInfo.nFileType = 2;

				fileInfo.nLength = (INT64)FindFileData.nFileSizeHigh << 32 | FindFileData.nFileSizeLow;

				FileTimeToLocalFileTime(&(FindFileData.ftLastWriteTime), &ftLocal);
				FileTimeToSystemTime(&ftLocal, &stTime);
				fileInfo.editTime.Format(_T("%u-%0.2d-%0.2d %0.2d:%0.2d:%0.2d"), stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

				listFile.push_back(fileInfo);
			}
			else if (_tcscmp(L".", FindFileData.cFileName) != 0 && _tcscmp(L"..", FindFileData.cFileName) != 0)
			{
				fileInfo.nFileType = 1;

				FileTimeToLocalFileTime(&(FindFileData.ftLastWriteTime), &ftLocal);
				FileTimeToSystemTime(&ftLocal, &stTime);
				fileInfo.editTime.Format(_T("%u-%0.2d-%0.2d %0.2d:%0.2d:%0.2d"), stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);

				listFile.push_back(fileInfo);
			}
		}
		while(FindNextFile(hFind,&FindFileData));
		FindClose(hFind);
	}

	//tmpWow64.Open();		//打开重定向
}
