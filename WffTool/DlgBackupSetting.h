#pragma once

using namespace SOUI;

class CDlgBackupSetting : public SHostDialog
{
public:
	CDlgBackupSetting();

	void OnClose();
	void	OnShowWindow(BOOL bShow, UINT uStatus);
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);

	void OnOK();
	void OnBrowser();
	void OnChkPopup();
	void OnRadioCur();
	void OnRadioSet();


	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close",OnClose)
		EVENT_NAME_COMMAND(L"btn_cancel",OnClose)
		EVENT_NAME_COMMAND(L"btn_ok",OnOK)
		EVENT_NAME_COMMAND(L"IDC_CHECK_AUTOBAK",OnChkPopup)
		EVENT_NAME_COMMAND(L"IDC_RADIO_CURDIR",OnRadioCur)
		EVENT_NAME_COMMAND(L"IDC_RADIO_SETDIR",OnRadioSet)
		EVENT_NAME_COMMAND(L"IDC_BUTTON_BROWSE",OnBrowser)
		EVENT_MAP_END()
		// HostWnd����������Ϣ����
		BEGIN_MSG_MAP_EX(CDlgBackupSetting)
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

public:
	BOOL m_bAutoCheck;
	BOOL m_bSetBakPath;
	CString m_strBakPath;

private:
	SCheckBox* m_cAuto;
	SRadioBox* m_rCur;
	SRadioBox* m_rSet;
	SEdit* m_EditPath;
	SButton* m_BtnBroswer;
}; 

