#include "CodeTrans_win.h"


wstring wff::utf8_to_wide(string strUTF8)
{
	wstring strWIDE;
	if ( false == strUTF8.empty() )
	{
		int nLen = MultiByteToWideChar ( CP_UTF8, 0, strUTF8.c_str(), static_cast<int>( strUTF8.size() ), NULL, 0 ) ;
		if ( nLen > 0 )
		{
			wchar_t* pchWide = new wchar_t[nLen+1];
			memset(pchWide, 0, nLen);
			pchWide[nLen] = L'\0';
			MultiByteToWideChar ( CP_UTF8, 0, strUTF8.c_str(), static_cast<int>(strUTF8.size()), pchWide, nLen ) ;
			strWIDE = pchWide;
			delete[] pchWide;
		}
	}

	return strWIDE;
}
wstring wff::ascii_to_wide(string strASII)
{
	wstring strWIDE;
	if ( false == strASII.empty() )
	{
		int nLen = MultiByteToWideChar ( CP_ACP, 0, strASII.c_str(), static_cast<int>( strASII.size() ), NULL, 0 ) ;
		if ( nLen > 0 )
		{
			wchar_t* pchWide = new wchar_t[nLen+1];
			memset(pchWide, 0, nLen);
			pchWide[nLen] = L'\0';
			MultiByteToWideChar ( CP_ACP, 0, strASII.c_str(), static_cast<int>(strASII.size()), pchWide, nLen ) ;
			strWIDE = pchWide;
			delete[] pchWide;
		}
	}

	return strWIDE;
}
string wff::wide_to_utf8(wstring strWIDE)
{
	string strUTF8;
	if ( false == strWIDE.empty() )
	{
		int nLen = WideCharToMultiByte ( CP_UTF8, 0, strWIDE.c_str(), static_cast<int>(strWIDE.size()), NULL, 0, NULL, NULL ) ;
		if ( nLen > 0 )
		{
			char* pchUTF8 = new char[nLen+1];
			memset(pchUTF8, 0, nLen);
			pchUTF8[nLen] = '\0';
			nLen = WideCharToMultiByte ( CP_UTF8, 0,strWIDE.c_str(), static_cast<int>(strWIDE.size()), pchUTF8, nLen, NULL, NULL ) ;
			strUTF8 = pchUTF8;
			delete[] pchUTF8;
		}
	}

	return strUTF8;
}
string wff::wide_to_ascii(wstring strWIDE)
{
	string strASCII;
	if ( false == strWIDE.empty() )
	{
		int nLen = WideCharToMultiByte ( CP_ACP, 0, strWIDE.c_str(), static_cast<int>(strWIDE.size()), NULL, 0, NULL, NULL ) ;
		if ( nLen > 0 )
		{
			char* pchASCII = new char[nLen+1];
			memset(pchASCII, 0, nLen);
			pchASCII[nLen] = '\0';
			nLen = WideCharToMultiByte ( CP_ACP, 0,strWIDE.c_str(), static_cast<int>(strWIDE.size()), pchASCII, nLen, NULL, NULL ) ;
			strASCII = pchASCII;
			delete[] pchASCII;
		}
	}

	return strASCII;
}
