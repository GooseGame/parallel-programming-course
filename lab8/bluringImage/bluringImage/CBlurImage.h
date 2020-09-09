#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <Windows.h>
#include <vector>
#include <ctime>
#include "../../easyBMP/EasyBMP.h"

using namespace std;

struct ThreadInfo
{
	ThreadInfo(const ThreadInfo &thread) {};
	ThreadInfo() {};
	~ThreadInfo() {};
	BMP inputImage;
	BMP *outputImage;
	vector<vector<RGBApixel>> pixels;
	int index;
	int threadCount;
	clock_t startTime;
};

class CBlurImage
{
public:
	CBlurImage(const char* inputImage, const char* outputImage, int threadCount, int coreCount, vector<int> threadPriorities);
	void StarBluring();
private:
	const char* m_inputImage;
	const char* m_outputImage;
	int m_threadCount, m_coreCount;
	vector<int> m_threadPriorities = {};
};