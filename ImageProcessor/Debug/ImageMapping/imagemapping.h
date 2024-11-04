#pragma once

#ifndef IMAGEMAPPING_H
#define IMAGEMAPPING_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <Windows.h>

using namespace cv;
using namespace std;

class ImageMapping
{
public:
	static ImageMapping& GetInstance(const string &path);
	bool Init(size_t mapSize);
	void MapImageToMemory();
	void WriteMetaData();
	void ReadResultFromMemory();
	bool GetReadResult();

private:
	ImageMapping(const string &path);

	ImageMapping(const ImageMapping&) = delete;
	ImageMapping& operator=(const ImageMapping&) = delete;

	bool LoadImage();

	Mat image;
	string imagePath;
	int width;
	int height;
	int rowSize;
	bool readRes;
	
	HANDLE hMapFile;
	unsigned char* memMap;
	size_t memMapSize;

	static ImageMapping* instance;
};
#endif //IMAGEMAPPING