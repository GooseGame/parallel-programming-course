#include "CLogBuffer.h"
#include <iostream>

CLogBuffer::CLogBuffer(mutex& mtx)
	: m_mtx(mtx), m_logList(CLogList()), m_fileNum(1)
{
	overflowEvent = CreateEvent(nullptr, true, false, "overflowEvent");
	StartOverflowThread();
}

void CLogBuffer::Push(string value)
{
	m_mtx.lock();
	if (!m_logList.IsOverflow()) { m_logList.Add(value); }
	else { ResetOverflowThread(); }
	m_mtx.unlock();
}

void CLogBuffer::StartOverflowThread()
{
	overflowThread = CreateThread(nullptr, 0, &LogSizeMonitoringThread, static_cast<void*>(this), 0, nullptr);
}

void CLogBuffer::ResetOverflowThread()
{
	SetEvent(overflowEvent);
	DWORD waitResult = WaitForSingleObject(overflowThread, INFINITE);

	if (waitResult == WAIT_OBJECT_0)
	{
		ResetEvent(overflowEvent);
		StartOverflowThread();
	}
}

DWORD WINAPI CLogBuffer::LogSizeMonitoringThread(CONST LPVOID lp_param)
{
	CLogBuffer *buff = static_cast<CLogBuffer*>(lp_param);
	const DWORD waitResult = WaitForSingleObject(buff->overflowEvent, INFINITE);

	if (waitResult == WAIT_OBJECT_0)
	{
		buff->m_logList.Write(buff->m_fileNum);
		buff->m_fileNum++;
		buff->m_logList.~CLogList();
	}
	ExitThread(EXIT_SUCCESS);
}