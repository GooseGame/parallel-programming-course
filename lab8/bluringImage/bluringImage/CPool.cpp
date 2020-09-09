#include "CPool.h"

using namespace std;

static DWORD WINAPI makeThread(CONST LPVOID lpParam) {
	CTask* task = (CTask*)lpParam;
	task->ExecuteTask();
	ExitThread(0);
}

CPool::CPool(vector<CTask*> tasks, int threadCount)
{
	m_taskCount = tasks.size();
	m_handles = new HANDLE[m_taskCount];
	m_threadCount = threadCount;
	for (int i = 0; i < m_taskCount; i++) {
		m_handles[i] = CreateThread(NULL, 0, &makeThread, tasks[i], CREATE_SUSPENDED, NULL);
	}
}

void CPool::Execute() {
	int count = 0;
	for (int i = 0; i < m_taskCount; i++) {
		ResumeThread(m_handles[i]);
		count++;
		if (count == m_threadCount) {
			WaitForMultipleObjects((DWORD)(i + 1), m_handles, true, INFINITE);
			count = 0;
		}
	}
	WaitForMultipleObjects((DWORD)m_taskCount, m_handles, true, INFINITE);
}