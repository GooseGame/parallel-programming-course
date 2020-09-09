#pragma once
#include <Windows.h>
#include <mutex>
#include <stdlib.h>
#include "CLogList.h"

using namespace std;

class CLogBuffer
{
public:
	CLogBuffer(mutex& mtx);
	void Push(string value);
	void StartOverflowThread();
	void ResetOverflowThread();
private:
	static DWORD WINAPI LogSizeMonitoringThread(CONST LPVOID lp_param);
	CLogList m_logList;
	mutex& m_mtx;
	HANDLE overflowEvent, overflowThread;
	int m_fileNum;
};