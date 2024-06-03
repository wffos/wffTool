#pragma once

using namespace SOUI;

class CDlgFileTypeSetting : public SHostDialog
{
public:
	CDlgFileTypeSetting();

	void OnClose();
	void	OnShowWindow(BOOL bShow, UINT uStatus);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

	void OnOK();
	void OnAdd();
	void OnAddOK();
	void OnAddNO();
	void OnDel();

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close",OnClose)
		EVENT_NAME_COMMAND(L"btn_cancel",OnClose)
		EVENT_NAME_COMMAND(L"btn_ok",OnOK)
		EVENT_NAME_COMMAND(L"btn_add",OnAdd)
		EVENT_NAME_COMMAND(L"btn_addok",OnAddOK)
		EVENT_NAME_COMMAND(L"btn_addno",OnAddNO)
		EVENT_NAME_COMMAND(L"btn_del",OnDel)
		EVENT_MAP_END()
		// HostWnd宿主窗口消息处理
		BEGIN_MSG_MAP_EX(CDlgFileTypeSetting)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		CHAIN_MSG_MAP(SHostDialog)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()

		//因退出sip进程，文件拖拽或复制粘贴会导致资源管理器崩溃，在sip_platform文件夹下创建退出标志文件
		//DMFileOp.dll通过这个标志文件过滤文件拖拽或复制粘贴动作   czj 2019.7.4
		void CreateExitFile();
private:
	SHostWnd	*m_pMainWnd;

private:
	SListCtrl* m_pList;
}; 

