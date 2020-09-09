#pragma once
#include "CBlurImage.h"

class CTask
{
public:
	CTask(CBlurImage inputImage);
	void ExecuteTask();
private:
	CBlurImage m_blurTask;
};