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
		// HostWnd����������Ϣ����
		BEGIN_MSG_MAP_EX(CDlgFileTypeSetting)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		CHAIN_MSG_MAP(SHostDialog)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()

		//���˳�sip���̣��ļ���ק����ճ���ᵼ����Դ��������������sip_platform�ļ����´����˳���־�ļ�
		//DMFileOp.dllͨ�������־�ļ������ļ���ק����ճ������   czj 2019.7.4
		void CreateExitFile();
private:
	SHostWnd	*m_pMainWnd;

private:
	SListCtrl* m_pList;
}; 

