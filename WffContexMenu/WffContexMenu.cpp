// WffContexMenu.cpp: DLL 导出的实现。


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "WffContexMenu_i.h"
#include "dllmain.h"


using namespace ATL;

// 用于确定 DLL 是否可由 OLE 卸载。
_Use_decl_annotations_
STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow();
}

// 返回一个类工厂以创建所请求类型的对象。
_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - 向系统注册表中添加项。
_Use_decl_annotations_
STDAPI DllRegisterServer(void)
{
	// 注册对象、类型库和类型库中的所有接口
	HRESULT hr = _AtlModule.DllRegisterServer();

	//// 注册对象、类型库和类型库中的所有接口
	//HKEY hKey;
	//static char pszGUID[] = "{4516bc6a-a646-4ba0-bfa6-6e1baf550da8}";
	//if (RegCreateKeyA(HKEY_CLASSES_ROOT, "*\\shellex\\ContextMenuHandlers\\WffCtxMenu", &hKey) != ERROR_SUCCESS)
	//{
	//	return E_FAIL;
	//}
	//if (RegSetValueA(hKey, NULL, REG_SZ, pszGUID, (DWORD)strlen(pszGUID)) != ERROR_SUCCESS)
	//{
	//	RegCloseKey(hKey);
	//	return E_FAIL;
	//}
	//RegCloseKey(hKey);

	//// 注册对象、类型库和类型库中的所有接口
	//HKEY hKey1;
	////static char pszGUID[] = "{4516bc6a-a646-4ba0-bfa6-6e1baf550da8}";
	//if (RegCreateKeyA(HKEY_CLASSES_ROOT, "Folder\\shellex\\ContextMenuHandlers\\WffCtxMenu", &hKey1) != ERROR_SUCCESS)
	//{
	//	return E_FAIL;
	//}
	//if (RegSetValueA(hKey1, NULL, REG_SZ, pszGUID, (DWORD)strlen(pszGUID)) != ERROR_SUCCESS)
	//{
	//	RegCloseKey(hKey1);
	//	return E_FAIL;
	//}
	//RegCloseKey(hKey1);

	//// 注册对象、类型库和类型库中的所有接口
	//HKEY hKey2;
	////static char pszGUID[] = "{4516bc6a-a646-4ba0-bfa6-6e1baf550da8}";
	//if (RegCreateKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers\\    wffOverlayIcon", &hKey2) != ERROR_SUCCESS)
	//{
	//	return E_FAIL;
	//}
	//if (RegSetValueA(hKey2, NULL, REG_SZ, pszGUID, (DWORD)strlen(pszGUID)) != ERROR_SUCCESS)
	//{
	//	RegCloseKey(hKey2);
	//	return E_FAIL;
	//}
	//RegCloseKey(hKey2);

	return hr;
}

// DllUnregisterServer - 移除系统注册表中的项。
_Use_decl_annotations_
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

// DllInstall - 按用户和计算机在系统注册表中逐一添加/移除项。
STDAPI DllInstall(BOOL bInstall, _In_opt_  LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != nullptr)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{
		hr = DllRegisterServer();
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}


