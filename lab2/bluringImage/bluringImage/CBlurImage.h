#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <Windows.h>
#include "../../easyBMP/EasyBMP.h"

using namespace std;

struct ThreadInfo
{
	ThreadInfo(const ThreadInfo &thread) {};
	ThreadInfo() {};
	~ThreadInfo() {};
	BMP inputImage;
	BMP *outputImage;
	int index;
	int threadCount;
};

class CBlurImage
{
public:
	CBlurImage(const char* inputImage, const char* outputImage, int threadCount, int coreCount);
	void StarBluring();
private:
	const char* m_inputImage;
	const char* m_outputImage;
	int m_threadCount, m_coreCount;
};