#include "StdAfx.h"
#include "CommonFunc.h"
#include "../common/file/path.h"


void FileOperate(int typeOper, list<CString>& listFile)
{
	//ִ��ҵ���߼�
	if (typeOper == 1)		//��ӱ��
	{
		WffDbManager::getInstance().AddFileList(listFile);
	}
	else if (typeOper == 2)		//ȡ�����
	{
		WffDbManager::getInstance().DeleteFileList(listFile);
	}
	else if (typeOper == 3)		//����
	{
		BOOL bret = TRUE;
		WffDbManager::getInstance().FilterFileList(listFile);
		for (list<CString>::iterator itr = listFile.begin(); itr != listFile.end(); itr++)
		{
			//����
			if (itr->Right(4) == L".wff" || itr->Right(6) == L"wff.db")
			{
				continue;
			}

			wff::RemovePath(*itr);
		}
	}
	else if (typeOper == 4)		//������ļ���
	{
		BOOL bret = TRUE;
		listFile.sort();
		for (list<CString>::reverse_iterator itr = listFile.rbegin(); itr != listFile.rend(); itr++)
		{
			bool bHasChild = false;
			TCHAR szFind[MAX_PATH] = { 0 };
			_tcscpy_s(szFind, MAX_PATH - 1, (*itr + L"\\*.*"));
			WIN32_FIND_DATA FindFileData = { 0 };
			HANDLE hFind = INVALID_HANDLE_VALUE;
			hFind = ::FindFirstFile(szFind, &FindFileData);
			if (INVALID_HANDLE_VALUE != hFind)
			{
				do
				{
					if ('.' != FindFileData.cFileName[0])
					{
						bHasChild = true;
						break;
					}
				} while (FindNextFile(hFind, &FindFileData));
				FindClose(hFind);
			}

			if (!bHasChild)
			{
				//����
				if (itr->Right(4) == L".wff" || itr->Right(6) == L"wff.db")
				{
					return;
				}

				bret = wff::RemovePath(*itr);
				if (!bret)
				{
					CString strMsg;
					strMsg.Format(_T("delete fail:%s"), *itr);
					OutputDebugString(strMsg);
					//MoveFileEx(*itr, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
				}
			}
		}
	}
}