// WffDBManager.cpp : 定义静态库的函数。
//

#include "WffDBManager.h"
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp.lib ")
#include <Shlwapi.h>
#include <ShlObj.h>


WffDbManager::WffDbManager() :
	m_db(NULL)
{
}

WffDbManager::~WffDbManager()
{
}

const char* WffDbManager::GetError()
{
	return sqlite3_errmsg(m_db);
}

bool WffDbManager::OpenDB(const wchar_t* szDBName, bool bCreateIfNotExist)
{
	bool m_blink = false;

	if (bCreateIfNotExist)
	{
		WCHAR szPath[MAX_PATH] = { 0 };
		swprintf_s(szPath, MAX_PATH, L"%s", szDBName);
		PathRemoveFileSpec(szPath);
		SHCreateDirectoryEx(NULL, szPath, NULL);
		HANDLE hFile = CreateFile(szDBName, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(hFile);
	}

	int nOpenRtn = 0;
	//nOpenRtn = sqlite3_open16(szDBName, &m_db);
	if (!m_db)
	{
		nOpenRtn = sqlite3_open16(szDBName, &m_db);
		if (nOpenRtn == SQLITE_OK)
		{
			m_blink = true;
		}
	}
	else
	{
		m_blink = true;
	}

	wstring strSql = L"Create Table if not exists FileInfo(" \
		L"ID INTEGER PRIMARY KEY AUTOINCREMENT ,"\
		L"Path CHAR(255) NOT NULL)";

	ExcuteSQL(strSql.c_str());

	return m_blink;
}

bool WffDbManager::CloseDB()
{
	if (m_db)
	{
		if (sqlite3_close(m_db) == SQLITE_OK)
		{
			m_db = NULL;
			return true;
		}
	}

	return false;
}

bool WffDbManager::OpenProjDB(CString strFile)
{
	bool bRtn = false;

	while (true)
	{
		CString strPath = strFile.GetString();
		strPath += L"\\.wff\\wff.db";
		if (PathFileExists(strPath))
		{
			bRtn = OpenDB(strPath.GetString(), false);
			break;
		}
		int nPos = strFile.ReverseFind(L'\\');
		if (nPos == -1)
		{
			break;
		}
		strFile = strFile.Left(nPos);
		if (strFile.IsEmpty())
		{
			break;
		}
	}

	return bRtn;
}

bool WffDbManager::ExcuteSQL(const wchar_t* szSql)
{
	std::string strSql = wff::wide_to_utf8(szSql);		//将语句转换成UTF_8编码格式
	char* szErrMsg = NULL;
	int rc = sqlite3_exec(m_db, strSql.c_str(), NULL, NULL, &szErrMsg);
	if (rc != SQLITE_OK)
	{
		if (szErrMsg != NULL)
		{
			sqlite3_free(szErrMsg);
		}
		return false;
	}
	return true;
}

void WffDbManager::AddFile(wchar_t* szFile)
{
	wstring strSql = L"insert into FileInfo" \
		L"(Path) values('";
	strSql = strSql + szFile + L"')";

	ExcuteSQL(strSql.c_str());
}

void WffDbManager::AddFileList(list<CString>& lsFile)
{
	sqlite3_exec(m_db, "Begin;", 0, 0, 0);
	for each (auto file in lsFile)
	{
		if (!IsFilePathExist((wchar_t*)file.GetString()))
		{//判断是否是已添加文件
			AddFile((wchar_t*)file.GetString());
		}
	}
	sqlite3_exec(m_db, "COMMIT;", 0, 0, 0);
}

bool WffDbManager::IsFilePathExist(wchar_t* szFile)
{
	int nCount = 0;

	wstring strSql = L"select count(1) from FileInfo " \
		L"where Path='";
	strSql = strSql + szFile + L"'";

	int rc;
	sqlite3_stmt* stmt;
	std::string strSqlUtf8 = wff::wide_to_utf8(strSql.c_str());		//将查询语句转换成UTF_8编码格式
	rc = sqlite3_prepare(m_db, strSqlUtf8.c_str(), -1, &stmt, 0);
	if (rc == SQLITE_OK)
	{
		rc = sqlite3_step(stmt);
		string str;
		wstring strinfo;
		while (rc == SQLITE_ROW)
		{
			str.clear();
			str = (char*)sqlite3_column_text(stmt, 0);
			if (!str.empty())
			{
				nCount = atoi(str.c_str());
			}
			rc = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}

	if (nCount > 0)
	{
		return true;
	}

	return false;
}

bool WffDbManager::DeleteFileByPath(wchar_t* szFile)
{
	wstring strSql = L"delete from FileInfo where Path is '";
	strSql = strSql + szFile + L"'";
	return ExcuteSQL(strSql.c_str());
}

bool WffDbManager::DeleteFileList(list<CString>& lsFile)
{
	sqlite3_exec(m_db, "Begin;", 0, 0, 0);
	for each (auto file in lsFile)
	{
		DeleteFileByPath((wchar_t*)file.GetString());
	}
	sqlite3_exec(m_db, "COMMIT;", 0, 0, 0);
	return true;
}

bool WffDbManager::FilterFileList(list<CString>& lsFile)
{
	sqlite3_exec(m_db, "Begin;", 0, 0, 0);
	for (list<CString>::iterator itr = lsFile.begin(); itr != lsFile.end(); )
	{
		if (WffDbManager::getInstance().IsFilePathExist((wchar_t*)itr->GetString()))
		{
			itr = lsFile.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	sqlite3_exec(m_db, "COMMIT;", 0, 0, 0);
	return true;
}