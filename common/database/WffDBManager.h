#pragma once
#include "../../common/pattern/singleton.h"
#include "../../common_public/sqlite3/sqlite3.h"
#include "../../common/codetrans/CodeTrans_win.h"

#include<list>
#include<string>
#include<atlstr.h>
using namespace ATL;

typedef struct _MsgData
{
    int nNo;
    wchar_t szTime[20];
    wchar_t szUser[64];
    wchar_t szMsg[1024];
    int   nStat;
    int nDBorder;
}MsgData;

#ifdef __cplusplus
extern "C"
{
#endif

    class WffDbManager :public wff::singleton<WffDbManager>
    {
    public:
        WffDbManager();
        ~WffDbManager();

       const char* GetError();

        bool OpenDB(const wchar_t* szDBName, bool bCreateIfNotExist = true);

        bool CloseDB();

        bool OpenProjDB(CString strFile);

        bool ExcuteSQL(const wchar_t* szSql);

        void AddFile(wchar_t* szFile);
        void AddFileList(list<CString>& lsFile);

        bool IsFilePathExist(wchar_t* szFile);

        bool DeleteFileByPath(wchar_t* szFile);
        bool DeleteFileList(list<CString>& lsFile);
        bool FilterFileList(list<CString>& lsFile);

    private:
        sqlite3* m_db;
    };

#ifdef __cplusplus
}
#endif
