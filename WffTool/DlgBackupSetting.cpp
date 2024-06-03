#include "StdAfx.h"
#include "DlgBackupSetting.h"


CDlgBackupSetting::CDlgBackupSetting() : SHostDialog(L"layout:dlg_buckupsetting")
{
	m_rCur = NULL;
	m_rSet = NULL;
	m_EditPath = NULL;
	m_BtnBroswer = NULL;
}

void CDlgBackupSetting::OnShowWindow( BOOL bShow, UINT uStatus )
{
	if (bShow)
	{
		AnimateHostWindow(200, AW_BLEND); // 动画效果
	}
}

LRESULT CDlgBackupSetting::OnInitDialog( HWND hWnd, LPARAM lParam )
{
	m_cAuto = FindChildByName2<SCheckBox>(L"IDC_CHECK_AUTOBAK");
	m_rCur = FindChildByName2<SRadioBox>(L"IDC_RADIO_CURDIR");
	m_rSet = FindChildByName2<SRadioBox>(L"IDC_RADIO_SETDIR");
	m_EditPath = FindChildByName2<SEdit>(L"IDC_EDIT_BAKPATH");
	m_BtnBroswer = FindChildByName2<SButton>(L"IDC_BUTTON_BROWSE");

	CString FileName = GetAPPPath() + _T("EstSysSet.sys");	

	// 读取是否自动备份
	UINT uBak = GetPrivateProfileInt(_T("FILETYPE"),_T("BAK"),1,FileName);
	m_bAutoCheck = (uBak == 1) ? TRUE : FALSE;
	m_cAuto->SetCheck(m_bAutoCheck);

	m_rCur->EnableWindow(m_bAutoCheck, TRUE);
	m_rSet->EnableWindow(m_bAutoCheck, TRUE);

	GetPrivateProfileString(_T("FILETYPE"),_T("BAKPATH"),_T(""),m_strBakPath.GetBuffer(MAX_PATH),MAX_PATH,FileName);
	m_strBakPath.ReleaseBuffer();
	if (m_strBakPath == _T(""))
	{
		m_rCur->SetCheck(TRUE);
		m_EditPath->EnableWindow(FALSE, TRUE);
		m_BtnBroswer->EnableWindow(FALSE, TRUE);
	}
	else
	{
		m_rSet->SetCheck(TRUE);
		m_EditPath->SetWindowText(m_strBakPath.GetString());
		m_EditPath->EnableWindow(TRUE, TRUE);
		m_BtnBroswer->EnableWindow(TRUE, TRUE);
	}

	return 0;
}

void CDlgBackupSetting::OnOK()
{
	SStringT str = m_EditPath->GetWindowText();
	m_bAutoCheck = m_cAuto->IsChecked();
	m_bSetBakPath = m_rSet->IsChecked();
	m_strBakPath = str.GetBuffer(str.GetLength());
	str.ReleaseBuffer();
	// 获取路径
	CString FileName = GetAPPPath() + _T("EstSysSet.sys");
	// 是否自动备份
	if(m_bAutoCheck)
		WritePrivateProfileString(_T("FILETYPE"),_T("BAK"),_T("1"),FileName.GetString());
	else
		WritePrivateProfileString(_T("FILETYPE"),_T("BAK"),_T("0"),FileName.GetString());
	// 是否设定路径
	if (m_bSetBakPath)
	{
		WritePrivateProfileString(_T("FILETYPE"),_T("BAKPATH"),m_strBakPath.GetString(),FileName.GetString());
	}
	else
	{
		WritePrivateProfileString(_T("FILETYPE"),_T("BAKPATH"),_T(""),FileName.GetString());
	}

	__super::OnOK();
}

void CDlgBackupSetting::OnClose()
{
	__super::OnCancel();
}

void CDlgBackupSetting::OnBrowser()
{
	BROWSEINFO bi = {0};
	ITEMIDLIST *pItemIDList = NULL;
	TCHAR szPath[MAX_PATH] = {0};

	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot= NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("选择文件夹");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
	bi.lpfn = NULL;
	bi.iImage = 0;
	pItemIDList = SHBrowseForFolder(&bi);

	if(TRUE == SHGetPathFromIDList(pItemIDList, szPath))//获取了目的文件夹
	{
		m_strBakPath = szPath;
		m_EditPath->SetWindowText(m_strBakPath.GetString());
	}
}

void CDlgBackupSetting::OnChkPopup()
{
	m_bAutoCheck = m_cAuto->IsChecked();

	m_rCur->EnableWindow(m_bAutoCheck, TRUE);
	m_rSet->EnableWindow(m_bAutoCheck, TRUE);
	m_EditPath->EnableWindow(m_bAutoCheck, TRUE);
	m_BtnBroswer->EnableWindow(m_bAutoCheck, TRUE);
}

void CDlgBackupSetting::OnRadioCur()
{
	m_EditPath->EnableWindow(FALSE, TRUE);
	m_BtnBroswer->EnableWindow(FALSE, TRUE);
}
void CDlgBackupSetting::OnRadioSet()
{
	m_EditPath->EnableWindow(TRUE, TRUE);
	m_BtnBroswer->EnableWindow(TRUE, TRUE);
}
