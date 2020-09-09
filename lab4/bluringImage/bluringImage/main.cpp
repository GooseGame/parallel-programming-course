#include "pch.h"
#include "CBlurImage.h"
#include <ctime>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	clock_t start = clock();
	setlocale(LC_ALL, "russian");

	if (argc != 5) {
		if (argc == 2 && strcmp(argv[1], "/?") == 0)
		{
			cout << "First, enter: blurImage.exe <inputBmpImage> <outputBmpImage> <threadCount> <coreCount>" <<
				"\nNext, enter thread priorities, -1 - BELOW_NORMAL, 0 - NORMAL, 1 - ABOVE_NORMAL" <<
				"\nIntroduced values of thread priorities must correspond thread count" << endl;
			return 0;
		}
		cout << "Enter blurImage.exe <inputBmpImage> <outputBmpImage> <threadCount> <coreCount>" << 
			    "\nOr enter blurImage.exe /?, if you dont know how to use this program" << endl; 
		return 0;
	}

	int threadCount;
	int coreCount;
	const char* inputImage = NULL;
	const char* outputImage = NULL;
	try
	{
	    threadCount = stoi(argv[3]);
		coreCount = stoi(argv[4]);
		inputImage = argv[1];
		outputImage = argv[2];
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 0;
	}
	CBlurImage blurImage(inputImage, outputImage, threadCount, coreCount);
	blurImage.StarBluring(start);

	clock_t end = clock();
	cout << "Done\nTime left: " << difftime(end, start) << " ms" << endl;
}
