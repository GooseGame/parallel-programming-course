#include "CTask.h"

CTask::CTask(CBlurImage inputImage) : m_blurTask(inputImage) {}

void CTask::ExecuteTask()
{
	m_blurTask.StarBluring();
}