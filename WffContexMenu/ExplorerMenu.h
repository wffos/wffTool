// ExplorerMenu.h: CExplorerMenu 的声明

#pragma once
#include "resource.h"       // 主符号



#include "WffContexMenu_i.h"
#include "../common/database/WffDBManager.h"
#include "../common/file/FindFileInDir_win.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

extern HINSTANCE g_hInstance;

CRITICAL_SECTION g_cri;

// CExplorerMenu

class ATL_NO_VTABLE CExplorerMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExplorerMenu, &CLSID_ExplorerMenu>,
	public IDispatchImpl<IExplorerMenu, &IID_IExplorerMenu, &LIBID_WffContexMenuLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu,
    public IShellIconOverlayIdentifier
{
public:
	CExplorerMenu()
	{
        InitializeCriticalSection(&g_cri);
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CExplorerMenu)
	COM_INTERFACE_ENTRY(IExplorerMenu)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
    COM_INTERFACE_ENTRY(IContextMenu)
    COM_INTERFACE_ENTRY(IShellIconOverlayIdentifier)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        m_hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_EXPLORER_MENU));

       /* WCHAR buff[MAX_PATH] = { 0 };
        GetEnvironmentVariable(_T("ALLUSERSPROFILE"), buff, MAX_PATH);
        wstring strPath = buff;
        strPath += L"\\wff.db";
        WffDbManager::getInstance().OpenDB(strPath.c_str());*/

        return S_OK;
    }

    void FinalRelease()
    {
        if (m_hBitmap)
        {
            DeleteObject(m_hBitmap);
        }
        //WffDbManager::getInstance().CloseDB();
    }

public:

    STDMETHODIMP Initialize(
        /* [in] */ LPCITEMIDLIST pidlFolder,
        /* [in] */ IDataObject* pdtobj,
        /* [in] */ HKEY hkeyProgID)
    {
        HRESULT hr;
        UINT    nFileCount;
        UINT    nLen;

        FORMATETC fmt =
        {
            CF_HDROP,
            NULL,
            DVASPECT_CONTENT,
            -1,
            TYMED_HGLOBAL
        };

        STGMEDIUM sm =
        {
            TYMED_HGLOBAL
        };

        hr = pdtobj->GetData(&fmt, &sm);

        if (FAILED(hr))
        {
            return hr;
        }

        nFileCount = DragQueryFile((HDROP)sm.hGlobal, 0xFFFFFFFF, NULL, 0);

        if (nFileCount >= 1)
        {
            nLen = DragQueryFile((HDROP)sm.hGlobal, 0, m_pszFileName, sizeof(m_pszFileName));

            if (nLen > 0 && nLen < MAX_PATH)
            {
                m_pszFileName[nLen] = _T('\0');
                hr = S_OK;
            }
            else
            {
                hr = E_INVALIDARG;
            }
        }
        else
        {
            hr = E_INVALIDARG;
        }

        ReleaseStgMedium(&sm);

        return hr;
    }

    STDMETHOD(QueryContextMenu)(
        HMENU hmenu,
        UINT indexMenu,
        UINT idCmdFirst,
        UINT idCmdLast,
        UINT uFlags)
    {
        if (uFlags & CMF_DEFAULTONLY)
        {
            return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
        }

        //InsertMenu(hmenu, indexMenu, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);//分割线
        //InsertMenu(hmenu, indexMenu + 1, MF_STRING | MF_BYPOSITION, idCmdFirst + 102, _T("Wff菜单_1"));
        //InsertMenu(hmenu, indexMenu + 2, MF_STRING | MF_BYPOSITION, idCmdFirst + 103, _T("Wff菜单_2"));
        //InsertMenu(hmenu, indexMenu + 3, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);//分割线 
        //return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(103 + 1));
        MENUITEMINFO mii;
        memset((void*)&mii, 0, sizeof(mii));
        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_STRING | MIIM_CHECKMARKS | MIIM_ID | MIIM_STATE;
        mii.hbmpChecked = m_hBitmap;//这里用mii.hbmpChecked而不用mii.hbmpItem
        mii.hbmpUnchecked = m_hBitmap;
        mii.fState = MFS_ENABLED;

        InsertMenu(hmenu, indexMenu, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);//分割线
        mii.cch = lstrlen(L"文件管理");
        mii.dwTypeData = L"文件管理";
        mii.wID = idCmdFirst + 1;
        InsertMenuItem(hmenu, indexMenu + 1, TRUE, &mii);
        mii.cch = lstrlen(L"添加标记");
        mii.dwTypeData = L"添加标记";
        mii.wID = idCmdFirst + 2;
        InsertMenuItem(hmenu, indexMenu + 2, TRUE, &mii);
        InsertMenu(hmenu, indexMenu + 3, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);//分割线 

        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 3 + 1);
    }

    STDMETHOD(InvokeCommand)(
        LPCMINVOKECOMMANDINFO lpici)
    {
        if (LOWORD(lpici->lpVerb) == 1)
        {
            //在此处理点击事件.
            WCHAR buff[MAX_PATH] = { 0 };
            //获取当前程序的路径，而不是调用该程序的程序的路径，因为调用该程序的是资源管理器（explorer.exe）,直接过去路径的话，获取的是资源管理器的路径。
            GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), buff, MAX_PATH);
            WCHAR* nChar = wcsrchr(buff, L'\\');
            if (nChar != NULL)
            {//写入当前程序路径下的图标的名字
                wcscpy_s(nChar, MAX_PATH - wcslen(buff), L"\\wffTool.exe");
            }
            ShellExecute(NULL, _T("open"), buff, NULL, NULL, SW_SHOWNORMAL);
            return S_OK;
        }
        else if (LOWORD(lpici->lpVerb) == 2)
        {
            //在此处理点击事件.
            WCHAR buff[MAX_PATH] = { 0 };
            //获取当前程序的路径，而不是调用该程序的程序的路径，因为调用该程序的是资源管理器（explorer.exe）,直接过去路径的话，获取的是资源管理器的路径。
            GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), buff, MAX_PATH);
            WCHAR* nChar = wcsrchr(buff, L'\\');
            if (nChar != NULL)
            {//写入当前程序路径下的图标的名字
                wcscpy_s(nChar, MAX_PATH - wcslen(buff), L"\\wffTool.exe");
            }
            wstring strArg = L"-a ";
            strArg = strArg + m_pszFileName;
            ShellExecute(NULL, _T("open"), buff, strArg.c_str(), NULL, SW_SHOWNORMAL);
            return S_OK;
        }

        return E_FAIL;
    }

    STDMETHOD(GetCommandString)(
        UINT_PTR    idCmd,
        UINT        uType,
        UINT* pwReserved,
        LPSTR       pszName,
        UINT        cchMax)
    {
        if ((idCmd != 1001) && (idCmd != 1002))
        {
            return E_INVALIDARG;
        }

        switch (uType)
        {
        case GCS_HELPTEXTA:
            lstrcpynA(pszName, "This is a test info", cchMax);
            break;

        case GCS_HELPTEXTW:
            lstrcpyn((LPWSTR)pszName, L"This is a test info", cchMax);
            break;

        case GCS_VERBA:
            lstrcpynA(pszName, "This is a test info", cchMax);
            break;

        case GCS_VERBW:
            lstrcpyn((LPWSTR)pszName, L"This is a test info", cchMax);
            break;

        default:
            break;
        }

        return S_OK;
    }

    STDMETHOD(IsMemberOf)(
        /* [string][in] */ __RPC__in_string LPCWSTR pwszPath,
        /* [in] */ DWORD dwAttrib)
    {
        HRESULT hRef = S_FALSE;

        EnterCriticalSection(&g_cri);
        if (WffDbManager::getInstance().OpenProjDB(pwszPath))
        {
            if(WffDbManager::getInstance().IsFilePathExist((wchar_t*)pwszPath))
            {//判断是否是已添加文件
                hRef = S_OK;
            }
            WffDbManager::getInstance().CloseDB();
        }
        LeaveCriticalSection(&g_cri);

        return hRef;
    }

    STDMETHOD(GetOverlayInfo)(
        /* [size_is][string][out] */ __RPC__out_ecount_full_string(cchMax) LPWSTR pwszIconFile,
        /* [in] */ int cchMax,
        /* [out] */ __RPC__out int* pIndex,
        /* [out] */ __RPC__out DWORD* pdwFlags)
    {
        WCHAR* buff = new WCHAR[cchMax];
        //获取当前程序的路径，而不是调用该程序的程序的路径，因为调用该程序的是资源管理器（explorer.exe）,直接过去路径的话，获取的是资源管理器的路径。
        GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), buff, cchMax);

        WCHAR* nChar = wcsrchr(buff, L'\\');
        if (nChar != NULL)
        {//写入当前程序路径下的图标的名字
            wcscpy_s(nChar, cchMax - wcslen(buff), L"\\wff.ico");
            wcscpy_s(pwszIconFile, cchMax, buff);
        }

        //设置顺序
        *pIndex = 0;

        //标识所修改过的数据pwszIconFile 与 pIndex
        *pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;

        delete[] buff;
        //完成返回S_OK
        return S_OK;
    }

    STDMETHOD(GetPriority)(
        /* [out] */ __RPC__out int* pPriority)
    {
        *pPriority = 0;
        return S_OK;
    }

private:
    TCHAR m_pszFileName[MAX_PATH];
    HBITMAP m_hBitmap;

};

OBJECT_ENTRY_AUTO(__uuidof(ExplorerMenu), CExplorerMenu)
