#include "CBlurImage.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

mutex mtx;
CLogBuffer buff = CLogBuffer(mtx);

void blur(ThreadInfo* thread, int startPoint, int endPoint, clock_t startTime)
{
	// horizontal blur
	int blurRadius = 2;
	int width = thread->inputImage.TellWidth();
	int height = thread->inputImage.TellHeight();
	vector<RGBApixel> pixels = {};

	for (int i = 2; i < width - 2; i++)
	{
		for (int j = startPoint + 2; j < endPoint - 2; j++)
		{
			RGBApixel p1 = thread->inputImage.GetPixel(i - 2, j);
			RGBApixel p2 = thread->inputImage.GetPixel(i - 1, j);
			RGBApixel p3 = thread->inputImage.GetPixel(i, j);
			RGBApixel p4 = thread->inputImage.GetPixel(i + 1, j);
			RGBApixel p5 = thread->inputImage.GetPixel(i + 2, j);

			byte byteRed = (byte)((p1.Red + p2.Red + p3.Red + p4.Red + p5.Red) / 5);
			byte byteGreen = (byte)((p1.Green + p2.Green + p3.Green + p4.Green + p5.Green) / 5);
			byte byteBlue = (byte)((p1.Blue + p2.Blue + p3.Blue + p4.Blue + p5.Blue) / 5);

			RGBApixel bluredPixel;
			bluredPixel.Red = byteRed;
			bluredPixel.Green = byteGreen;
			bluredPixel.Blue = byteBlue;

			thread->outputImage->SetPixel(i, j, bluredPixel);
			clock_t endTime = clock();
			buff.Push(to_string(thread->index) + " " + to_string(difftime(endTime, thread->startTime)));
		}
	}

	//vertical blur
	for (int i = 0; i < width; i++)
	{
		for (int j = startPoint + 2; j < endPoint - 2; j++)
		{
			RGBApixel p1 = thread->inputImage.GetPixel(i, j - 2);
			RGBApixel p2 = thread->inputImage.GetPixel(i, j - 1);
			RGBApixel p3 = thread->inputImage.GetPixel(i, j);
			RGBApixel p4 = thread->inputImage.GetPixel(i, j + 1);
			RGBApixel p5 = thread->inputImage.GetPixel(i, j + 2);

			byte byteRed = (byte)((p1.Red + p2.Red + p3.Red + p4.Red + p5.Red) / 5);
			byte byteGreen = (byte)((p1.Green + p2.Green + p3.Green + p4.Green + p5.Green) / 5);
			byte byteBlue = (byte)((p1.Blue + p2.Blue + p3.Blue + p4.Blue + p5.Blue) / 5);

			RGBApixel bluredPixel;
			bluredPixel.Red = byteRed;
			bluredPixel.Green = byteGreen;
			bluredPixel.Blue = byteBlue;

			thread->outputImage->SetPixel(i, j, bluredPixel);
		}
	}
}

DWORD WINAPI MakeThread(CONST LPVOID lpParam)
{
	ThreadInfo* thread = static_cast<ThreadInfo*>(lpParam);
	int width = thread->inputImage.TellWidth();
	int height = thread->inputImage.TellHeight();
	int threadCount = thread->threadCount;
	int threadIndex = thread->index;
	clock_t startTime = thread->startTime;
	int startPointIndex = (height / threadCount) * (threadIndex - 1);
	int endPointIndex = (height / threadCount) * threadIndex;

	blur(thread, startPointIndex, endPointIndex, startTime);
	ExitThread(0);
}

bool CBlurImage::isPrioritiesReaded()
{
	string threadPrioritiesLine = "";
	string buff = "";
	vector<string> threadPrioritiesString = {};
	vector<int> threadPrioritiesInt = {};
	cout << "Enter thread priorities\n-1 - BELOW_NORMAL, 0 - NORMAL, 1 - ABOVE_NORMAL\nYou must enter " << m_threadCount << " values" << endl;
	getline(cin, threadPrioritiesLine);
	stringstream ss(threadPrioritiesLine);
	while (ss >> buff)
	{
		threadPrioritiesString.push_back(buff);
	}
	if (threadPrioritiesString.size() != m_threadCount)
	{
		cout << "Number of values not equal number of threads" << endl;
		return false;
	}

	for (int i = 0; i < threadPrioritiesString.size(); i++)
	{
		int threadPriority;
		try
		{
			threadPriority = stoi(threadPrioritiesString.at(i));
			threadPrioritiesInt.push_back(threadPriority);
		}
		catch (exception e)
		{
			cout << e.what() << "\nValues must be a number" << endl;
			return false;
		}
	}

	for (int i = 0; i < threadPrioritiesInt.size(); i++)
	{
		if ((threadPrioritiesInt.at(i) != 0) && (threadPrioritiesInt.at(i) != -1) && (threadPrioritiesInt.at(i) != 1))
		{
			cout << "Invalid thread priority\nPermissible values: -1, 0, 1" << endl;
			return false;
		}
	}
	m_threadPriorities = threadPrioritiesInt;
	return true;
}

void CBlurImage::StarBluring(clock_t start) {
	BMP inputImage;
	BMP *outputImage = new BMP();
	if (!inputImage.ReadFromFile(m_inputImage)) { exit(0); };
	outputImage->ReadFromFile(m_inputImage);

	if (!isPrioritiesReaded()) { exit(0); }
	vector<vector<RGBApixel>> pixels = {};
	for (int i = 0; i < inputImage.TellWidth(); i++)
	{
		pixels.push_back({});
		for (int j = 0; j < inputImage.TellHeight(); j++)
		{
			pixels.at(0).push_back(inputImage.GetPixel(i, j));
		}
	}
	cout << "Opened " << m_inputImage << endl;
	cout << "Start bluring..." << endl;
	vector<ThreadInfo*> threads = {};
	for (int i = 0; i < m_threadCount; ++i)
	{
		ThreadInfo *thread = new ThreadInfo();
		thread->index = i + 1;
		thread->inputImage = inputImage;
		thread->outputImage = outputImage;
		thread->threadCount = m_threadCount;
		thread->startTime = start;
		thread->pixels = pixels;
		threads.push_back(thread);
	}
	vector<HANDLE> handles(threads.size());
	int affinityMask = (1 << m_coreCount) - 1;
	for (size_t i = 0; i < threads.size(); ++i)
	{
		handles[i] = CreateThread(NULL, 0, &MakeThread, threads[i], CREATE_SUSPENDED, NULL);
		SetThreadPriority(handles.at(i), m_threadPriorities.at(i));
		SetThreadAffinityMask(handles[i], affinityMask);
	}

	for (const auto& handle : handles)
	{
		ResumeThread(handle);
	}

	WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
	threads.at(0)->outputImage->WriteToFile(m_outputImage);
}

CBlurImage::CBlurImage(const char* inputImage, const char* outputImage, int threadCount, int coreCount)
	: m_inputImage(inputImage), m_outputImage(outputImage), m_threadCount(threadCount), m_coreCount(coreCount)
{}
