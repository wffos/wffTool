#ifndef	_WFF_LOG_H_
#define	_WFF_LOG_H_

#include <atlstr.h>
#include <share.h>
#include <shlobj.h>
#include <Shlwapi.h>
#include <atltrace.h>
#include <locale.h>

#include "../process/AutoLock_win.h"

#define MAX_LOG_SIZE 4096

CString GetExeName();

BOOL WriteLogA(const CHAR* strLevel, UINT uLineNum, const CHAR* strFileName, const CHAR* strFunName, const CHAR* strLog, ...);

BOOL WriteLogW(const WCHAR* strLevel, UINT uLineNum, const WCHAR* strFileName, const WCHAR* strFunName, const WCHAR* strLog, ...);

#define WFF_LOG(level, logformat, ...) \
	do {\
		if (sizeof(logformat[0]) == sizeof(char)) {\
			WriteLogA(level, __LINE__, (__FILE__), (__FUNCTION__), (const char*)logformat, ##__VA_ARGS__);\
		}else{\
			USES_CONVERSION;\
			WriteLogW(A2W(level), __LINE__, A2W(__FILE__), A2W(__FUNCTION__), (const wchar_t*)logformat, ##__VA_ARGS__);\
		}\
	} while (0);

#define WFF_LOG_INFO(fmt, ...)  WFF_LOG("INFO", fmt,  ##__VA_ARGS__)
#define WFF_LOG_WARN(fmt, ...)  WFF_LOG("WARN" , fmt,  ##__VA_ARGS__)
#define WFF_LOG_ERROR(fmt, ...) WFF_LOG("ERROR" fmt,  ##__VA_ARGS__)



#endif	// _WFF_LOG_H_