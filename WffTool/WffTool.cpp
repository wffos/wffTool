// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "MainDlg.h"

#ifdef _DEBUG
#ifdef _WIN64
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced64.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#endif
#else
#ifdef _WIN64
#define SYS_NAMED_RESOURCE _T("soui-sys-resource64.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
#endif

//定义唯一的一个R,UIRES对象,ROBJ_IN_CPP是resource.h中定义的宏。
ROBJ_IN_CPP

unsigned __stdcall FuncAddFile(CString pPara)
{
	TCHAR szFileName[MAX_PATH] = { 0 };
	swprintf_s(szFileName, MAX_PATH, L"%s", pPara.GetString());
	if (PathIsDirectory(szFileName))
	{
		list<CString> vFile;
		wff::FindFileInDir(vFile, szFileName, L"*.*");
		WffDbManager::getInstance().AddFileList(vFile);
	}
	else
	{
		if (!WffDbManager::getInstance().IsFilePathExist(szFileName))
		{//判断是否是已添加文件
			WffDbManager::getInstance().AddFile(szFileName);
		}
	}

	return 0;
}

int ShowMainDlg(HINSTANCE hInstance)		//弹出主界面
{
	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));

	int nRet = 0;

	SComMgr* pComMgr = new SComMgr;

	{
		BOOL bLoaded = FALSE;
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));

		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		SApplication* theApp = new SApplication(pRenderFactory, hInstance);

		HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
		if (hModSysResource)
		{
			CAutoRefPtr<IResProvider> sysResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
			sysResProvider->Init((WPARAM)hModSysResource, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
			FreeLibrary(hModSysResource);
		}

		CAutoRefPtr<IResProvider>   pResProvider;

		CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
		TCHAR szPath[MAX_PATH] = { 0 };
		SStringT strCurPath = theApp->GetAppDir();
		swprintf_s(szPath, MAX_PATH - 1, _T("%s\\Skin\\WffToolSkin"), strCurPath.GetBuffer(0));
		strCurPath.ReleaseBuffer();
		bLoaded = pResProvider->Init((LPARAM)szPath, 0);
		SASSERT(bLoaded);

		theApp->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
		theApp->AddResProvider(pResProvider);

		//加载多语言翻译模块。
		CAutoRefPtr<ITranslatorMgr> trans;
		bLoaded = pComMgr->CreateTranslator((IObjRef**)&trans);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("translator"));
		if (trans)
		{//加载语言翻译包
			theApp->SetTranslator(trans);
			pugi::xml_document xmlLang;
			if (theApp->LoadXmlDocment(xmlLang, _T("lang_cn"), _T("translator")))
			{
				CAutoRefPtr<ITranslator> langCN;
				trans->CreateTranslator(&langCN);
				langCN->Load(&xmlLang.child(L"language"), 1);//1=LD_XML
				trans->InstallTranslator(langCN);
			}
		}

		{
			CMainDlg dlgMain;
			dlgMain.Create(GetActiveWindow());
			dlgMain.SendMessage(WM_INITDIALOG);
			dlgMain.CenterWindow(dlgMain.m_hWnd);
			dlgMain.ShowWindow(/*SW_SHOWMAXIMIZED*/SW_SHOWNORMAL);
			nRet = theApp->Run(dlgMain.m_hWnd);
		}

		delete theApp;
	}

	delete pComMgr;

	OleUninitialize();
	return nRet;
}

int ParseCommandLine(LPTSTR lpstrCmdLine)
{
	wstring command = lpstrCmdLine;
	if (command.find(L"-c") == 0)
	{
		CTime Mytime2 = CTime::GetCurrentTime();
		CString strFile = command.substr(3, command.length()).c_str();
		if (WffDbManager::getInstance().OpenProjDB(strFile))
		{
			WffDbManager::getInstance().CloseDB();
			return -1;
		}
		strFile = strFile + L"\\wffProj_" + Mytime2.Format(L"%y%m%d%H%M%S").GetString();;
		CreateDirectory(strFile, NULL);
		strFile = strFile + L"\\.wff";
		CreateDirectory(strFile, NULL);
		strFile = strFile + L"\\wff.db";
		HANDLE hFile = CreateFile(strFile, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return -2;
		}
	}
	else if(command.find(L"-a") == 0)
	{
		CString strFile = command.substr(3, command.length()).c_str();
		if(WffDbManager::getInstance().OpenProjDB(strFile))
		{
			FuncAddFile(strFile);
			WffDbManager::getInstance().CloseDB();
		}
	}

	return 1;
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	/*HANDLE m_hMutex = CreateMutex(NULL, FALSE, _T("WFF_MyExplorer"));
	if (m_hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 1;
	}*/

	if (_tcslen(lpstrCmdLine))
	{
		ParseCommandLine(lpstrCmdLine);
	}
	else
	{
		return ShowMainDlg(hInstance);
	}
}
