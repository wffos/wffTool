#pragma once
#include <Windows.h>
#include <string>
#include <locale>

using namespace std;

namespace wff
{
	wstring utf8_to_wide(string strUTF8);
	wstring ascii_to_wide(string strASII);
	string wide_to_utf8(wstring strWIDE);
	string wide_to_ascii(wstring strWIDE);
}
