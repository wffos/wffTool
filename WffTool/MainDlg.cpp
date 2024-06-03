// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include "helper\SMenu.h"

unsigned __stdcall RefreshData(void * Param)
{
	CMainDlg* pDlg = (CMainDlg*)Param;
	pDlg->m_bShowMask = true;
	pDlg->m_strMaskText = L"正在刷新，请稍等...";
	PostMessage(pDlg->m_hWnd, WM_SHOWMASK,NULL,NULL);
	
	WffDbManager::getInstance().CloseDB();
	pDlg->m_bOpendProjDB = WffDbManager::getInstance().OpenProjDB(pDlg->m_strPath);

	pDlg->m_listFile.clear();
	if (pDlg->m_mapFile.end() != pDlg->m_mapFile.find(pDlg->m_strPath))
	{
		pDlg->m_listFile.splice(pDlg->m_listFile.begin(), pDlg->m_mapFile[pDlg->m_strPath]);
	}
	else
	{
		FindFileInDir(pDlg->m_strPath, pDlg->m_listFile);
	}

	pDlg->m_mapFile.clear();
	list<FileInfo> listFile;
	for (list<FileInfo>::iterator itr = pDlg->m_listFile.begin(); itr != pDlg->m_listFile.end(); itr++)
	{
		listFile.clear();
		FindFileInDir(itr->strFullPath.GetString(), listFile);
		pDlg->m_mapFile.insert(make_pair(itr->strFullPath, listFile));
	}

	PostMessage(pDlg->m_hWnd, WM_REFRESH_WINDOW, NULL, NULL);		//刷新界面

	pDlg->m_bShowMask = false;
	pDlg->m_strMaskText = L"";
	PostMessage(pDlg->m_hWnd, WM_SHOWMASK,NULL,NULL);

	return 0;
}

unsigned __stdcall FuncFileOperate(void* Param)
{
	CMainDlg* pDlg = (CMainDlg*)Param;
	pDlg->m_bShowMask = true;
	pDlg->m_strMaskText = L"操作正在进行，请稍等...";
	PostMessage(pDlg->m_hWnd, WM_SHOWMASK, NULL, NULL);

	FileOperate(pDlg->m_typeOper, pDlg->m_listPath);
	if (pDlg->m_typeOper == 3)		//清理时，同时清除空文件夹
	{
		FileOperate(4, pDlg->m_listDir);
	}
	PostMessage(pDlg->m_hWnd, WM_REFRESH_WINDOW, NULL, NULL);		//刷新界面

	pDlg->m_bShowMask = false;
	pDlg->m_strMaskText = L"";
	PostMessage(pDlg->m_hWnd, WM_SHOWMASK, NULL, NULL);

	return 0;
}

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:dlg_main"))
{
	m_bLayoutInited = FALSE;
	m_pTree = NULL;
	m_rootItem = 0;
	m_pList = NULL;
	m_bShowMask = false;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ModifyStyle(WS_BORDER, 0);	//去掉overlap风格窗口的默认圆角
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	m_pTree=this->FindChildByName2<STreeCtrl>(L"tree_left");
	m_pList=this->FindChildByName2<SListCtrl>(L"lc_all");

	if (m_pTree)
	{
		m_pTree->GetEventSet()->subscribeEvent(EVT_TC_SELCHANGED,Subscriber(&CMainDlg::OnTreeSelectChange,this));

		m_rootItem = m_pTree->GetRootItem();
		HSTREEITEM tempItem;

		TCHAR pszLink[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_DESKTOP, NULL, SHGFP_TYPE_CURRENT, pszLink);
		tempItem = m_pTree->InsertItem(pszLink, 1, 1, m_rootItem, STVI_LAST, TRUE);
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszLink);
		tempItem = m_pTree->InsertItem(pszLink, 2, 2, m_rootItem, STVI_LAST, TRUE);

		vector<CString> arDriverName;
		CString strDriverName;
		TCHAR szDriverName[500];
		TCHAR szDriverLabel[500];
		memset(szDriverName,0,sizeof(szDriverName));
		memset(szDriverLabel,0,sizeof(szDriverLabel));
		DWORD nLength = GetLogicalDriveStrings(sizeof(szDriverName),szDriverName);
		int i=0;
		for (; i<(int)nLength; i++)
		{
			if (szDriverName[i] != '\0')
				strDriverName += szDriverName[i];
			else
			{
				strDriverName = strDriverName.Left(strDriverName.GetLength() - 1);
				arDriverName.push_back(strDriverName);
				strDriverName = _T("");
			}
		}

		int nDriverType;
		TCHAR *szCurDir = NULL;
		for (i=0; i<arDriverName.size(); i++)
		{
			nDriverType = GetDriveType((LPCTSTR)arDriverName.at(i));
			GetVolumeInformation((LPCTSTR)(arDriverName.at(i)),szDriverLabel,sizeof(szDriverLabel),NULL,NULL,0,NULL,0);
			tempItem = m_pTree->InsertItem(arDriverName.at(i), 3, 3, m_rootItem, STVI_LAST, TRUE);
		}
	}

	return 0;
}

void CMainDlg::OnLanguage(int nID)
{
	ITranslatorMgr *pTransMgr = SApplication::getSingletonPtr()->GetTranslator();
	SASSERT(pTransMgr);
	std::wstring strLang;
	switch(nID)
	{
	case 0:
		strLang = _T("lang_cn");
		break;
	case 1:
		strLang = _T("lang_jp");
		break;
	default:
		strLang = _T("lang_cn");
		break;
	}

	pugi::xml_document xmlLang;
	if (SApplication::getSingletonPtr()->LoadXmlDocment(xmlLang, strLang.c_str(), _T("translator")))
	{
		CAutoRefPtr<ITranslator> lang;
		pTransMgr->CreateTranslator(&lang);
		lang->Load(&xmlLang.child(L"language"), 1);//1=LD_XML
		TCHAR lngName[TR_MAX_NAME_LEN] = {0};
		lang->GetName(lngName);
		pTransMgr->SetLanguage(lngName);
		pTransMgr->InstallTranslator(lang);
		SDispatchMessage(UM_SETLANGUAGE,0,0);
	}
}

//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnDrop()
{
	CPoint pt;
	GetCursorPos(&pt);
	SMenu menu;
	menu.LoadMenu(_T("menu_drop"),_T("SMENU"));
	menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

bool CMainDlg::Refresh(HSTREEITEM nItem)		//nItem: 树节点
{
	SStringT strName, strText;
	HSTREEITEM tmpItem = nItem;
	while (m_rootItem != tmpItem)
	{
		m_pTree->GetItemText(tmpItem, strText);
		strName = strText + L"\\" + strName;
		tmpItem = m_pTree->GetParentItem(tmpItem);
	}
	if (strName.GetLength()-1 == strName.ReverseFind('\\'))
	{
		strName = strName.Left(strName.GetLength()-1);
	}

	m_nSel = nItem;
	m_strPath = strName.GetBuffer(strName.GetLength());
	strName.ReleaseBuffer();

	HANDLE hdl = (HANDLE)_beginthreadex(NULL, 0, RefreshData, this, 0, NULL);
	if (hdl)
	{
		CloseHandle(hdl);
	}

	return true;
}

bool CMainDlg::OnTreeSelectChange(EventTCSelChanged *pEvt)
{
	if (pEvt->hNewSel != pEvt->hOldSel)
		return Refresh(pEvt->hNewSel);
	return false;
}

void CMainDlg::OnListMenu(EventArgs *pEvt)
{
	EventCtxMenu *pEvt2 = sobj_cast<EventCtxMenu>(pEvt);
	POINT pt = pEvt2->pt;

	{
		//选中鼠标点击行
		CPoint pt2 = pt;
		int iItem = m_pList->HitTest(pt2);
		ClientToScreen(&pt);
		m_pList->SetSelectedItem(iItem);
		if (iItem == -1)
		{
			return;
		}
	}

	SMenu menu;
	menu.LoadMenu(_T("menu_rightlist"), _T("SMENU"));
	menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);

}

void CMainDlg::OnTreeMenu(EventArgs *pEvt)
{
	EventCtxMenu *pEvt2 = sobj_cast<EventCtxMenu>(pEvt);
	POINT pt = pEvt2->pt;

	{
		//选中鼠标点击行
		CPoint pt2 = pt;
		HSTREEITEM iItem = m_pTree->HitTest(pt2);
		ClientToScreen(&pt);
		m_pTree->SelectItem(iItem);
		if (iItem == 0)
		{
			return;
		}
	}

	SMenu menu;
	menu.LoadMenu(_T("menu_lefttreet"),_T("SMENU"));
	menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);

}

void CMainDlg::OnListDBClick(EventArgs* pEvt)
{
	EventLCDbClick* pEvt2 = sobj_cast<EventLCDbClick>(pEvt);
	SStringT strTextClick = m_pList->GetSubItemText(pEvt2->nCurSel, 0);

	SStringT strText1 = m_pList->GetSubItemText(pEvt2->nCurSel, 1);
	SStringT strText2 = m_pList->GetSubItemText(pEvt2->nCurSel, 2);
	SStringT strText3 = m_pList->GetSubItemText(pEvt2->nCurSel, 3);
	if (strText1 != L"" || strText2 != L"" || strText3 != L"")
	{
		return;
	}

	//获取树信息
	SStringT strText = L"";
	int nItem = m_pTree->GetChildItem(m_nSel);
	while (nItem != -1)
	{
		m_pTree->GetItemText(nItem, strText);
		if (strText == strTextClick)
		{
			m_pTree->SelectItem(nItem);
			break;
		}
		nItem = m_pTree->GetNextSiblingItem(nItem);
	}
}

bool CMainDlg::GetTreeSelect(list<CString>& listPath, bool bDir)
{
	//获取树勾选信息
	int nSel = m_pTree->GetSelectedItem();
	if (nSel != -1)
	{
		SStringT strName, strText;
		HSTREEITEM tmpItem = nSel;
		while (m_rootItem != tmpItem)
		{
			m_pTree->GetItemText(tmpItem, strText);
			strName = strText + L"\\" + strName;
			tmpItem = m_pTree->GetParentItem(tmpItem);
		}
		if (strName.GetLength()-1 == strName.ReverseFind('\\'))
		{
			strName = strName.Left(strName.GetLength()-1);
		}
		if (bDir)
		{
			wff::FindDirInDir(listPath, strName.GetBuffer(0));
		}
		else
		{
			wff::FindFileInDir(listPath, strName.GetBuffer(0), L"*.*");
		}
		strName.ReleaseBuffer();
	}

	return true;
}

bool CMainDlg::GetListSelect(list<CString>& listPath)
{
	bool bHasDir = false;
	//获取界面勾选信息
	SStringT strFullPath;
	int nItemS = m_pList->GetFirstCheckedItem();
	int nItemE = m_pList->GetLastCheckedItem();
	if (nItemS != -1 || nItemE != -1)
	{
		while (nItemE >= nItemS)
		{
			if (m_pList->GetCheckState(nItemS) == TRUE)
			{
				strFullPath = m_pList->GetSubItemText(nItemS, 4);
				if (PathIsDirectory(strFullPath))
				{
					bHasDir = true;
					list<CString> vFile;
					wff::FindFileInDir(listPath, strFullPath.GetBuffer(0), L"*.*");
					strFullPath.ReleaseBuffer();
				}
				else
				{
					listPath.push_back(strFullPath.GetBuffer(strFullPath.GetLength()));
					strFullPath.ReleaseBuffer();
				}
			}
			nItemS ++;
		}
	}

	return bHasDir;
}

void CMainDlg::FileOperateByCommond(int nTypeBy, int typeOper)
{
	if (nTypeBy == 0)
	{
		FileOperateByTree(typeOper);
	}
	else if (nTypeBy == 1)
	{
		FileOperateByList(typeOper);
	}
}
void CMainDlg::FileOperateByTree(int typeOper)
{
	m_listPath.clear();
	GetTreeSelect(m_listPath);
	if (typeOper == 3)
	{
		m_listDir.clear();
		GetTreeSelect(m_listDir, true);
	}
	m_typeOper = typeOper;		//标记、取消标记操作类型

	FileOperateByThread();
}
void CMainDlg::FileOperateByList(int typeOper)
{
	m_listPath.clear();
	GetListSelect(m_listPath);
	m_typeOper = typeOper;		//标记、取消标记操作类型
	FileOperateByThread();
}

void CMainDlg::FileOperateByThread()
{
	HANDLE hdl = (HANDLE)_beginthreadex(NULL, 0, FuncFileOperate, this, 0, NULL);
	if (hdl)
	{
		CloseHandle(hdl);
	}
}

//DUI菜单响应函数
void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if(uNotifyCode==0)
	{
		if (nID == 31)
		{
			OnLanguage(0);
		}
		else if (nID == 32)
		{
			OnLanguage(1);
		}
		else if (nID == 100)		//添加标记
		{
			FileOperateByCommond(1, 1);
		}
		else if (nID == 110)		//取消标记
		{
			if (m_bOpendProjDB)
			{
				FileOperateByCommond(1, 2);
			}
		}
		else if (nID == 200)		//添加标记
		{
			FileOperateByCommond(0, 1);
		}
		else if (nID == 210)		//取消标记
		{
			if (m_bOpendProjDB)
			{
				FileOperateByCommond(0, 2);
			}
		}
		else if (nID == 220)		//清理 //清理空文件夹
		{
			if (m_bOpendProjDB)
			{
				FileOperateByCommond(0, 3);
			}
		}
		else if (nID == 230)		//刷新
		{
			Refresh(m_pTree->GetSelectedItem());
		}
	}
}

LRESULT CMainDlg::OnShowMaks(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL)
{
	SWindow* pMask = this->FindChildByName(L"mask");
	if (pMask)
	{
		pMask->SetVisible(m_bShowMask, TRUE);
	}

	SWindow* pText = this->FindChildByName(L"msg");
	if (pText)
	{
		pText->SetWindowText(m_strMaskText.GetString());
	}

	return 0;
}

LRESULT CMainDlg::OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL)
{
	AddTree(m_nSel, m_listFile, 2);		//文件树刷新
	AddList(m_nSel, m_listFile);			//文件表刷新

	return 0;
}

bool CMainDlg::AddTree(HSTREEITEM hItem, list<FileInfo>& listFile, int nDeep)
{
	if (!m_pTree)
	{
		return false;
	}

	if (--nDeep < 0)
	{
		return true;
	}

	if (m_rootItem == hItem)
	{
		return true;
	}

	//文件树
	SStringT strRoot, strName, strText;
	HSTREEITEM tmpItem = hItem;
	while (m_rootItem != tmpItem)
	{
		m_pTree->GetItemText(tmpItem, strText);
		strName = strText + L"\\" + strName;
		tmpItem = m_pTree->GetParentItem(tmpItem);
	}
	while(true)
	{
		tmpItem = m_pTree->GetChildItem(hItem);
		if (tmpItem == 0)
		{
			break;
		}
		m_pTree->RemoveItem(tmpItem);
	}

	for (list<FileInfo>::iterator itr = listFile.begin(); itr != listFile.end(); itr++)
	{
		if (itr->nFileType != 1)		//文件树只显示文件夹
		{
			continue;
		}
		tmpItem = m_pTree->InsertItem(itr->strName.GetString(), 4, 4, hItem, STVI_LAST, TRUE);
		if (m_mapFile.end() != m_mapFile.find(itr->strFullPath))
		{
			AddTree(tmpItem, m_mapFile[itr->strFullPath], nDeep);
		}
	}
	m_pTree->Expand(hItem, TVE_COLLAPSE);

	return true;
}

bool CMainDlg::AddList(HSTREEITEM hItem, list<FileInfo>& listFile)
{
	if (!m_pList)
	{
		return false;
	}

	if (m_rootItem == hItem)
	{
		return true;
	}

	//文件表
	SStringT strRoot, strName, strText;
	HSTREEITEM tmpItem = hItem;
	while (m_rootItem != tmpItem)
	{
		m_pTree->GetItemText(tmpItem, strText);
		strName = strText + L"\\" + strName;
		tmpItem = m_pTree->GetParentItem(tmpItem);
	}
	m_pList->DeleteAllItems();
	int i = 0;
	for (list<FileInfo>::reverse_iterator itr = listFile.rbegin(); itr != listFile.rend(); itr++)
	{
		int nItem = m_pList->InsertItem(i, itr->strName);

		CString strType = L"", strFilelen =L"";
		
		if(itr->nFileType == 2)
		{
			if (WffDbManager::getInstance().IsFilePathExist((wchar_t*)itr->strFullPath.GetString()))
			{
				strType = L"已标记";
			}

			if(itr->nLength<1024)
				strFilelen.Format(_T("%d Bytes"),(DWORD)itr->nLength);
			else if(itr->nLength>=1024 && itr->nLength<1024*1024)
			{
				ULONGLONG nTemp = 1024;
				strFilelen.Format(_T("%0.02f K"),(double)(__int64)itr->nLength/(__int64)nTemp);
			}
			else if(itr->nLength>=1024*1024 && itr->nLength<1024*1024*1024)
			{
				ULONGLONG nTemp = 1024*1024;
				strFilelen.Format(_T("%0.02f M"),(double)(__int64)itr->nLength/(__int64)nTemp);
			}
			else if (itr->nLength>=1024*1024*1024)
			{
				ULONGLONG nTemp = 1024*1024*1024;
				double duRet = (double)(__int64)itr->nLength/(__int64)nTemp;
				strFilelen.Format(_T("%0.02f G"),duRet);
			}
		}

		m_pList->SetSubItemText(nItem,1, strType);
		m_pList->SetSubItemText(nItem,2, strFilelen);
		m_pList->SetSubItemText(nItem,3, itr->editTime);
		m_pList->SetSubItemText(nItem,4, itr->strFullPath);

	}

	return true;
}