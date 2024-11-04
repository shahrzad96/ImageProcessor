#include "imagemapping.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

ImageMapping* ImageMapping::instance = nullptr;

ImageMapping& ImageMapping::GetInstance(const string &path)
{
	if (!instance)
	{
		instance = new ImageMapping(path);
	}

	return *instance;
}

ImageMapping::ImageMapping(const string &path)
	:imagePath(path), width(0), height(0), rowSize(0), hMapFile(nullptr), memMap(nullptr), memMapSize(0), readRes(false)
{
	LoadImage();
}

bool ImageMapping::LoadImage()
{
	image = cv::imread(imagePath, cv::IMREAD_COLOR);
	if (image.empty())
	{
		cerr << "failed to load image at " << imagePath << "\n";
		return false;
	}

	width = image.cols;
	height = image.rows;
	rowSize = width * 3;
	//set the last byte as the statusFlag indicating wether pixel color calculation is concluded
	memMapSize = sizeof(int) * 3 + (rowSize * height) + sizeof(int);

	//to store the memory map size and pass it to other processes.
	std::ofstream sizeFile("map_size.txt");
	if (sizeFile.is_open()) {
		sizeFile << memMapSize;
		sizeFile.close();
	}
	else {
		std::cerr << "Failed to write map size to file." << std::endl;
		return -1;
	}

	return true;
}

bool ImageMapping::Init(size_t mapSize)
{
	memMapSize = mapSize + sizeof(int);

	hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, memMapSize, L"Local\\ImageMap");
	if (hMapFile == NULL)
	{
		cerr << "Couldn't create a file mapping object " << GetLastError() << "\n";
		return false;
	}

	memMap = (unsigned char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, memMapSize);
	if (memMap == NULL)
	{
		cerr << "couldn't map view of file " << GetLastError() << " \n";
		CloseHandle(hMapFile);
		return false;
	}

	return true;
}

void ImageMapping::WriteMetaData()
{
	int* metaData = reinterpret_cast<int*>(memMap);
	metaData[0] = memMapSize;
	metaData[1] = width;
	metaData[2] = height;
}

void ImageMapping::MapImageToMemory()
{
	WriteMetaData();

	//write image row data to memory
	for (int i = 0; i < height; i++)
	{
		memcpy(memMap + sizeof(int) * 3 + i * rowSize, image.ptr(i), rowSize);
		cout << "\r" << (i * 100) / height << "%" << flush;
		Sleep(10);
	}
	cout << std::endl;
}

void ImageMapping::ReadResultFromMemory()
{
	unsigned char* color;
	int* statusFlag = reinterpret_cast<int*>(memMap + memMapSize - sizeof(int));

	if(*statusFlag == 1)
	{ 
		readRes = true;
		//Read the BGR data in RGB
		for (int i = 0; i < height; i++)
		{
			color = memMap + i * 3;
			cout << "Most common color for row " << i << ": ("
				<< (int)color[2] << ", "
				<< (int)color[1] << ", "
				<< (int)color[0] << ") \n";
		}
	}
	else
	{
		Sleep(10);
	}
}

bool ImageMapping::GetReadResult()
{
	return readRes;
}