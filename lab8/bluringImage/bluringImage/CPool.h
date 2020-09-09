#pragma once
#include <vector>
#include "CTask.h"

using namespace std;

class CPool
{
public:
	CPool(vector<CTask*> tasks, int threadCount);
	void Execute();
private:
	HANDLE *m_handles;
	int m_threadCount, m_taskCount;
};