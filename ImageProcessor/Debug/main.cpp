#include "ImageMapping/imagemapping.h"
#include <fstream>

using namespace std;

int main()
{
	string imagePath = "";
	cout << "Enter image path:";
	getline(std::cin, imagePath);

	cout << "Attempting to load image from: " << imagePath << std::endl;

	cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
	if (image.empty()) {
		std::cerr << "Failed to load image at " << imagePath << std::endl;
		return -1;
	}

	size_t imageWidth = 0;
	size_t imageHeight = 0;

	cout << "Enter image width: ";
	if (1 != scanf_s("%zu", &imageWidth)) {
		cerr << "Invalid width input.\n";
		return -1;
	}

	cout << "Enter image height: ";
	if (1 != scanf_s("%zu", &imageHeight)) {
		cerr << "Invalid height input.\n";
		return -1;
	}

	const size_t mapSize = imageWidth * imageHeight * 3;

	ImageMapping& mapper = ImageMapping::GetInstance(imagePath);
	if (!mapper.Init(mapSize))
	{
		cerr << "Failed to initialize memory map \n";
		return -1;
	}

	mapper.MapImageToMemory();
	while (!mapper.GetReadResult())
	{
		mapper.ReadResultFromMemory();
	}

	return 0;
}