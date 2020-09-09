#include "CLogList.h"
#include <fstream>
#include <vector>

CLogList::CLogList()
	: head(NULL), tail(NULL), m_size(0) {}

CLogList::~CLogList()
{
	while (m_size != 0)
	{
		Node *temp = head->next;
		delete head;
		head = temp;

		m_size--;
	}
}

void CLogList::Add(string value)
{
	m_size++;

	Node *temp = new Node;
	temp->next = head;
	temp->logLine = value;

	if (head != NULL)
	{
		tail->next = temp;
		tail = temp;
	}
	else
	{
		head = tail = temp;
	}
}

int CLogList::GetSize()
{
	return m_size;
}

void CLogList::Write(int threadNum)
{
	ofstream outputFile("log" + to_string(threadNum) + ".txt");
	Node *tempHead = new Node;
	tempHead = head;
	vector<string> logValues = {};

	int temp = m_size;
	while (temp != 0)
	{
		logValues.push_back(tempHead->logLine);
		tempHead = tempHead->next;
		temp--;
	}
	//reverse(logValues.begin(), logValues.end());
	for (int i = 0; i < logValues.size(); i++)
	{
		outputFile << logValues.at(i) << endl;
	}
	outputFile.close();

}

bool CLogList::IsOverflow()
{
	return m_size > MAX_LIST_SIZE;
}