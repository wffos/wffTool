#include "FindFileInDir_win.h"

void wff::FindExeInCurDir(CString strDir, list<CString>& vFile)
{
	if (MAX_PATH - 10 <= strDir.GetLength())
	{
		return;
	}

	CString strSrcPath = strDir;
	CString strExtension;
	TCHAR szFind[MAX_PATH] = {0};
	_tcscpy_s(szFind, MAX_PATH, (strSrcPath + L"\\*.exe"));
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}
			strExtension = PathFindExtension(FindFileData.cFileName);
			if(L".exe" == strExtension.MakeLower())
			{
				vFile.push_back(strDir + L"\\" + FindFileData.cFileName);
			}	
		}
		while(FindNextFile(hFind,&FindFileData));

		FindClose(hFind);
	}
}

void wff::FindFileInCurDir(list<CString>& vFile, CString strDir, CString strFilter)
{
	if (MAX_PATH <= strDir.GetLength() + strFilter.GetLength() + 2)
	{
		return;
	}

	TCHAR szFind[MAX_PATH] = {0};
	_tcscpy_s(szFind, MAX_PATH - 1, (strDir + L"\\" + strFilter));
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind=::FindFirstFile(szFind, &FindFileData);
	if(INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)&&(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (_tcscmp(L".", FindFileData.cFileName) != 0 && _tcscmp(L"..", FindFileData.cFileName) != 0)
				{
					vFile.push_back(strDir + L"\\" + FindFileData.cFileName);
				}
			}
		}
		while(FindNextFile(hFind,&FindFileData));

		FindClose(hFind);
	}
}

void wff::FindFileInDir(list<CString>& vFile, CString strDir, CString strFilter)			//
{
	if (MAX_PATH <= strDir.GetLength() + 5)
	{
		return;
	}

	FindFileInCurDir(vFile, strDir, strFilter);

	TCHAR szFind[MAX_PATH] = {0};
	_tcscpy_s(szFind, MAX_PATH - 1, (strDir + L"\\*.*"));
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind=::FindFirstFile(szFind, &FindFileData);
	if(INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)&&(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}
			if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(L".", FindFileData.cFileName) != 0 && _tcscmp(L"..", FindFileData.cFileName) != 0)
				{
					FindFileInDir(vFile, strDir + L"\\" + FindFileData.cFileName, strFilter);	
				}	
			}
		}
		while(FindNextFile(hFind,&FindFileData));
		FindClose(hFind);
	}
}

void wff::FindDirInCurDir(list<CString>& vDir, CString strDir)
{
	if (MAX_PATH <= strDir.GetLength() + 2)
	{
		return;
	}

	TCHAR szFind[MAX_PATH] = { 0 };
	_tcscpy_s(szFind, MAX_PATH - 1, (strDir + L"\\*.*"));
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (_tcscmp(L".", FindFileData.cFileName) != 0 && _tcscmp(L"..", FindFileData.cFileName) != 0)
				{
					vDir.push_back(strDir + L"\\" + FindFileData.cFileName);
				}
			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}
}


void wff::FindDirInDir(list<CString>& vDir, CString strDir)
{
	if (MAX_PATH <= strDir.GetLength() + 5)
	{
		return;
	}

	FindDirInCurDir(vDir, strDir);

	TCHAR szFind[MAX_PATH] = { 0 };
	_tcscpy_s(szFind, MAX_PATH - 1, (strDir + L"\\*.*"));
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(L".", FindFileData.cFileName) != 0 && _tcscmp(L"..", FindFileData.cFileName) != 0) 
				{
					FindDirInDir(vDir, strDir + L"\\" + FindFileData.cFileName);
				}
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
}

void FindAllFileInDir(CString strDir, list<CString>& listFile, bool bChild)
{
	BOOL bIsEncryptFile = FALSE;
	CString strSrcPath = strDir;
	TCHAR szFind[MAX_PATH] = { 0 };
	_tcscpy_s(szFind, MAX_PATH, (strDir + L"\\*.*"));
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (_tcslen(FindFileData.cFileName) == 0)
			{
				continue;
			}
			/*if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				continue;
			}*/
			strSrcPath = strDir + L"\\" + FindFileData.cFileName;
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				listFile.push_back(strSrcPath);
			}
			else if (_tcscmp(L".", FindFileData.cFileName) != 0 && _tcscmp(L"..", FindFileData.cFileName) != 0)
			{
				if (bChild)
					FindAllFileInDir(strSrcPath, listFile, bChild);
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
}