#pragma once
#include <stdlib.h>
#include <string>

using namespace std;
const int MAX_LIST_SIZE = 10000;

struct Node
{
	string logLine;
	Node *next;
};

class CLogList
{
private:
	Node *head, *tail;
	int m_size;
public:
	CLogList();
	~CLogList();
	void Add(string value);
	int GetSize();
	void Write(int threadNum);
	bool IsOverflow();
};
