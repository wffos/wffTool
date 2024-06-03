#include "log.h"


CTCritSec g_sec;

BOOL WriteLogA(const CHAR* strLevel, UINT uLineNum, const CHAR* strFile, const CHAR* strFunName, const CHAR* strLog, ...)
{
	try
	{
		CAutoLock lock(&g_sec);
		do
		{
			SYSTEMTIME m_stime;
			GetLocalTime(&m_stime);

			CStringA szPrefix;
			char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
			setlocale(LC_CTYPE, "chs");

			SYSTEMTIME	st = { 0 };
			GetLocalTime(&st);
			CStringA strFileName = strFile;
			strFileName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\') - 1);
			szPrefix.Format("[%02d:%02d:%02d:%03d] [%d] [%d] [%s] [%s] [%d] [%s] : ",
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				GetCurrentProcessId(), GetCurrentThreadId(), strFileName, strFunName, uLineNum, strLevel);

			CHAR szBuf[MAX_PATH] = { 0 };
			GetModuleFileNameA(NULL, szBuf, MAX_PATH);
			CStringA strExe = szBuf;
			CStringA strExePath = strExe.Left(strExe.ReverseFind('\\'));
			CStringA strExeName = strExe.Right(strExe.GetLength() - strExe.ReverseFind('\\') - 1);
			strExeName = strExeName.Left(strExeName.ReverseFind('.'));

			strFileName.Format("%s\\log\\%s_%d-%d-%d%s", strExePath, strExeName, st.wYear, st.wMonth, st.wDay, ".log");

			va_list argp;
			va_start(argp, strLog);

			FILE* m_pfLogFile = _fsopen(strFileName.GetString(), "a", _SH_DENYNO);
			if (m_pfLogFile)
			{
				fprintf(m_pfLogFile, szPrefix);
				vfprintf(m_pfLogFile, strLog, argp);
				fprintf(m_pfLogFile, "\n");
				fflush(m_pfLogFile);
				fclose(m_pfLogFile);
			}

			va_end(argp);

			setlocale(LC_CTYPE, old_locale);
			free(old_locale);

		} while (0);

		return TRUE;
	}
	catch (...)
	{
		OutputDebugStringA("catch(...) CEstLog::WriteLogW");
	}

	return FALSE;
}

BOOL WriteLogW(const WCHAR* strLevel, UINT uLineNum, const WCHAR* strFile, const WCHAR* strFunName, const WCHAR* strLog, ...)
{
	try
	{
		CAutoLock lock(&g_sec);
		do
		{
			SYSTEMTIME m_stime;
			GetLocalTime(&m_stime);

			CString szPrefix;
			char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
			setlocale(LC_CTYPE, "chs");

			SYSTEMTIME	st = { 0 };
			GetLocalTime(&st);
			CString strFileName = strFile;
			strFileName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\') - 1);
			szPrefix.Format(_T("[%02d:%02d:%02d:%03d] [%d] [%d] [%s] [%s] [%d] [%s] : "), 
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				GetCurrentProcessId(), GetCurrentThreadId(), strFileName, strFunName, uLineNum, strLevel);

			TCHAR szBuf[MAX_PATH] = { 0 };
			GetModuleFileName(NULL, szBuf, MAX_PATH);
			CString strExe = szBuf;
			CString strExePath = strExe.Left(strExe.ReverseFind('\\'));
			CString strExeName = strExe.Right(strExe.GetLength() - strExe.ReverseFind('\\') - 1);
			strExeName = strExeName.Left(strExeName.ReverseFind('.'));

			strFileName.Format(_T("%s\\log\\%s_%d-%d-%d%s"), strExePath, strExeName, st.wYear, st.wMonth, st.wDay, L".log");

			va_list argp;
			va_start(argp, strLog);

			FILE* m_pfLogFile = _wfsopen(strFileName.GetString(), _T("a"), _SH_DENYNO);
			if (m_pfLogFile)
			{
				fwprintf(m_pfLogFile, szPrefix);
				vfwprintf(m_pfLogFile, strLog, argp);
				fwprintf(m_pfLogFile, _T("\n"));
				fflush(m_pfLogFile);
				fclose(m_pfLogFile);
			}

			va_end(argp);

			setlocale(LC_CTYPE, old_locale);
			free(old_locale);

		} while (0);

		return TRUE;
	}
	catch (...)
	{
		OutputDebugStringA("catch(...) CEstLog::WriteLogW");
	}

	return FALSE;
}

