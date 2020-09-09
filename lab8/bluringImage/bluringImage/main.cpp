#include "pch.h"
#include "CBlurImage.h"
#include "CTask.h"
#include "CPool.h"
#include <ctime>
#include <string>
#include <sstream>
#include <filesystem>
#include <experimental/filesystem>

using namespace std;

const int DEFAULT_MODE = 0;
const int POOL_MODE = 1;

vector<int> GetThreadPriorities(int threadCount)
{
	//----------
	string threadPrioritiesLine = "";
	string buff = "";
	vector<string> threadPrioritiesString = {};
	vector<int> threadPrioritiesInt = {};
	cout << "Enter thread priorities\n-1 - BELOW_NORMAL, 0 - NORMAL, 1 - ABOVE_NORMAL\nYou must enter " << threadCount << " values" << endl;
	getline(cin, threadPrioritiesLine);
	stringstream ss(threadPrioritiesLine);
	while (ss >> buff)
	{
		threadPrioritiesString.push_back(buff);
	}
	if (threadPrioritiesString.size() != threadCount)
	{
		cout << "Number of values not equal number of threads" << endl;
		exit(0);
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
			exit(0);
		}
	}

	for (int i = 0; i < threadPrioritiesInt.size(); i++)
	{
		if ((threadPrioritiesInt.at(i) != 0) && (threadPrioritiesInt.at(i) != -1) && (threadPrioritiesInt.at(i) != 1))
		{
			cout << "Invalid thread priority\nPermissible values: -1, 0, 1" << endl;
			exit(0);
		}
	}
	return threadPrioritiesInt;
	//----------
}

int main(int argc, char* argv[])
{
	clock_t start = clock();
	setlocale(LC_ALL, "russian");

	if (argc != 6) {
		if (argc == 2 && strcmp(argv[1], "/?") == 0)
		{
			cout << "First, enter: blurImage.exe <inputBmpImage> <outputBmpImage> <threadCount> <coreCount> <mode>" <<
				"\n  Mode: 0 - default mode, 1 - pool mode " <<
				"\nNext, enter thread priorities, -1 - BELOW_NORMAL, 0 - NORMAL, 1 - ABOVE_NORMAL" <<
				"\nIntroduced values of thread priorities must correspond thread count" << endl;
			return 0;
		}
		cout << "Enter blurImage.exe <inputBmpImage> <outputBmpImage> <threadCount> <coreCount> <mode>" <<
			    "Mode: 0 - default mode, 1 - pool mode " <<
			    "\nOr enter blurImage.exe /?, if you dont know how to use this program" << endl;
		return 0;
	}

	int threadCount;
	int coreCount;
	int mode;
	const char* inputImageDirectory;
	const char* outputImageDirectory;

	try
	{
	    threadCount = stoi(argv[3]);
		coreCount = stoi(argv[4]);
		inputImageDirectory = argv[1];
		outputImageDirectory = argv[2];
		mode = stoi(argv[5]);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 0;
	}

	vector<int> threadPriorities = GetThreadPriorities(threadCount);

	if (!experimental::filesystem::exists(inputImageDirectory)) {
		cout << "Directory of input images does not exist" << endl;
		return 1;
	}

	if (!experimental::filesystem::exists(outputImageDirectory)) {
		experimental::filesystem::create_directories(outputImageDirectory);
	}

	vector<string> inputImagesPaths;
	vector<string> outputImagesPaths;
	for (const auto& file : experimental::filesystem::directory_iterator(inputImageDirectory)) {
		auto path = file.path();
		if (path.extension() == ".bmp") {
			inputImagesPaths.push_back(path.string());
			string outputImagePath = outputImageDirectory;
			outputImagePath += "/blured_" + path.stem().string() + ".bmp";
			outputImagesPaths.push_back(outputImagePath);
		}
	}

	if (mode == DEFAULT_MODE)
	{
		for (int i = 0; i < inputImagesPaths.size(); i++)
		{
			CBlurImage blurImage(inputImagesPaths.at(i).c_str(), outputImagesPaths.at(i).c_str(), threadCount, coreCount, threadPriorities);
			blurImage.StarBluring();
		}
	}
	else if (mode == POOL_MODE)
	{
		vector<CTask*> tasks;
		for (int i = 0; i < inputImagesPaths.size(); i++)
		{
			CBlurImage blurImage(inputImagesPaths.at(i).c_str(), outputImagesPaths.at(i).c_str(), threadCount, coreCount, threadPriorities);
			tasks.push_back(new CTask(blurImage));
		}
		CPool pool(tasks, threadCount);
		pool.Execute();
	}
	else
	{
		cout << "Unknown mode" << endl;
		return 1;
	}

	clock_t end = clock();
	cout << "Done\nTime left: " << difftime(end, start) << " ms" << endl;
}
