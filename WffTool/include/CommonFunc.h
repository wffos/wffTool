#pragma once


void FileOperate(int typeOper, list<CString>& listFile);

class CTCritSec
{
public:
	CTCritSec()
	{
		InitializeCriticalSection(&m_CritSec);
	};
	~CTCritSec()
	{
		DeleteCriticalSection(&m_CritSec);
	};

	void Lock()
	{
		EnterCriticalSection(&m_CritSec);
	};
	void Unlock()
	{
		LeaveCriticalSection(&m_CritSec);
	};

	CTCritSec &operator=(const CTCritSec &refCritSec)
	{
		m_CritSec=refCritSec.m_CritSec;
		return *this;
	};

	CRITICAL_SECTION m_CritSec;
};

class CAutoLock 
{
public:
	CAutoLock(CTCritSec * plock)
	{
		m_pLock = plock;
		m_pLock->Lock();
	};

	~CAutoLock() 
	{
		m_pLock->Unlock();
	};

	CAutoLock &operator=(const CAutoLock &refAutoLock)
	{
		m_pLock=refAutoLock.m_pLock;
		m_pLock->Lock();

		return *this;
	};

protected:
	CTCritSec * m_pLock;
};