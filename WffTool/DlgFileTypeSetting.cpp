#include "StdAfx.h"
#include "DlgFileTypeSetting.h"


CDlgFileTypeSetting::CDlgFileTypeSetting() : SHostDialog(L"layout:dlg_filetypesetting")
{
	m_pList = NULL;
}

void CDlgFileTypeSetting::OnShowWindow( BOOL bShow, UINT uStatus )
{
	if (bShow)
	{
		AnimateHostWindow(200, AW_BLEND); // 动画效果
	}
}

LRESULT CDlgFileTypeSetting::OnInitDialog( HWND hWnd, LPARAM lParam )
{
	m_pList = FindChildByName2<SListCtrl>(L"lc_all");

	CString strFilePath = GetCurrentDirForUser() + _T("EstSysSet.sys");

	TCHAR szFilePath[4096];
	int num = GetPrivateProfileString(_T("FILETYPE"), _T("FileExt"), _T(""),szFilePath,4096,strFilePath);
	if(num >= 4096)
	{
		MessageBoxA(NULL, "类型过多，无法取全","提示",MB_OK);
	}

	CString strFileTypes = szFilePath;
	int nFind = strFileTypes.Find('|');
	while (nFind > 0)
	{
		CString strTmp = strFileTypes.Left(nFind);
		m_pList->InsertItem(m_pList->GetItemCount(),strTmp);
		strFileTypes = strFileTypes.Right(strFileTypes.GetLength() - nFind -1);
		nFind = strFileTypes.Find('|');
	}

	return 0;
}

void CDlgFileTypeSetting::OnOK()
{
	CString strFileTypes;
	CString FileName = GetCurrentDirForUser() + _T("EstSysSet.sys");	
	int nCount = m_pList->GetItemCount();
	for(int i=0;i<m_pList->GetItemCount();i++)
	{
		SStringT strExt = m_pList->GetSubItemText(i, 0);
		strFileTypes = strFileTypes + strExt.GetBuffer(strExt.GetLength()) + _T("|"); 
		strExt.ReleaseBuffer();
		nCount --;
	}
	WritePrivateProfileString(_T("FILETYPE"),_T("FileExt"),strFileTypes.GetString(),FileName);

	__super::OnOK();
}

void CDlgFileTypeSetting::OnClose()
{
	__super::OnCancel();
}

void CDlgFileTypeSetting::OnAdd()
{
	FindChildByName2<SWindow>(L"mask")->SetVisible(TRUE, TRUE);
}

void CDlgFileTypeSetting::OnAddOK()
{
	SStringT strAdd = FindChildByName2<SWindow>(L"edit_add")->GetWindowText();
	m_pList->InsertItem(m_pList->GetItemCount(),strAdd);
	FindChildByName2<SWindow>(L"mask")->SetVisible(FALSE, TRUE);
}

void CDlgFileTypeSetting::OnAddNO()
{
	FindChildByName2<SWindow>(L"mask")->SetVisible(FALSE, TRUE);
}

void CDlgFileTypeSetting::OnDel()
{
	int nCount = m_pList->GetFirstCheckedItem();
	while (nCount != -1)
	{
		m_pList->DeleteItem(nCount);
		nCount = m_pList->GetFirstCheckedItem();
	}
}