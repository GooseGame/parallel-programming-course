#include "CBlurImage.h"
#include <ctime>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	clock_t start = clock();
	setlocale(LC_ALL, "russian");

	if (argc != 5) { cout << "Enter blurImage.exe <inputBmpImage> <outputBmpImage> <threadCount> <coreCount>" << endl; return 0; }

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
	}
	CBlurImage blurImage(inputImage, outputImage, threadCount, coreCount);
	blurImage.StarBluring();

	clock_t end = clock();
	cout << "Done\nTime left: " << difftime(end, start) << " ms" << endl;
}
