#pragma once
#include <Windows.h>
#include <atlstr.h>
#include <list>

using namespace std;
using namespace ATL;

namespace wff
{
	void FindExeInCurDir(CString strDir, list<CString>& vFile);
	void FindFileInCurDir(list<CString>& vFile, CString strDir, CString strFilter);
	void FindFileInDir(list<CString>& vFile, CString strDir, CString strFilter);
	void FindDirInCurDir(list<CString>& vDir, CString strDir);
	void FindDirInDir(list<CString>& vDir, CString strDir);
}
