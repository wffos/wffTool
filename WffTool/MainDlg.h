// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnDrop();
	void OnSize(UINT nType, CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

protected:

	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_drop", OnDrop)
		EVENT_NAME_HANDLER(L"lc_all",EVT_CTXMENU,OnListMenu)
		EVENT_NAME_HANDLER(L"tree_left",EVT_CTXMENU,OnTreeMenu)
		EVENT_NAME_HANDLER(L"lc_all", EVT_LC_DBCLICK, OnListDBClick)
	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_COMMAND(OnCommand)
		MESSAGE_HANDLER(WM_SHOWMASK, OnShowMaks)
		MESSAGE_HANDLER(WM_REFRESH_WINDOW, OnRefresh)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	void OnLanguage(int nID);

	bool OnTreeSelectChange(EventTCSelChanged *pEvt);
	void OnListMenu(EventArgs *pEvt);
	void OnTreeMenu(EventArgs *pEvt);
	void OnListDBClick(EventArgs* pEvt);

	//DUI菜单响应函数
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);

	LRESULT OnShowMaks(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL);
	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL);

	bool AddTree(HSTREEITEM hItem, list<FileInfo>& listFile, int nDeep);
	bool AddList(HSTREEITEM hItem, list<FileInfo>& listFile);

private:
	bool GetTreeSelect(list<CString>& listPath, bool bDir = false);
	bool GetListSelect(list<CString>& listPath);		//有文件夹勾选返回true
	bool FilterByExt(list<CString>& listPath);

	void FileOperateByCommond(int nTypeBy, int typeOper);
	void FileOperateByTree(int typeOper);
	void FileOperateByList(int typeOper);
	void FileOperateByThread();

	bool Refresh(HSTREEITEM nItem);		//nItem: 树节点

public:
	bool m_bShowMask;
	CString m_strMaskText;

	HSTREEITEM m_nSel;		//当前叶子
	CString m_strPath;		//当前叶子路径
	list<FileInfo> m_listFile;						//当前叶子的子项
	map<CString, list<FileInfo>> m_mapFile;		//叶子子项的子项

	int m_typeOper;					//加解密操作类型
	list<CString> m_listPath;		//需要操作的文件
	list<CString> m_listDir;		//需要操作的文件夹

	bool m_bOpendProjDB;		//数据库

private:
	BOOL m_bLayoutInited;	

	STreeCtrl * m_pTree;
	HSTREEITEM m_rootItem;
	SListCtrl * m_pList;

};
