#include "BMPLoader.h"
#include <iostream>

ResourceManager gResourceManager;

std::string BMPLoader::GetColourCharacter(unsigned char b, unsigned char g, unsigned char r)
{
	if (r == 255	&& g == 0	&& b == 0)		return "red";
	if (r == 0		&& g == 255 && b == 0)		return "grn";
	if (r == 0		&& g == 0	&& b == 255)	return "blu";
	if (r == 255	&& g == 255 && b == 255)	return "wht";
	if (r == 255	&& g == 255 && b == 0)		return "ylw";
	if (r == 0		&& g == 0	&& b == 0)		return "blk";
	return "xxx";
}

BMPImage* BMPLoader::createColorVector()
{
	bmp = new BMPImage("zBMPLoader\\resourceMap.bmp");

	char* data = bmp->GetData();
	unsigned long imageWidth = bmp->GetImageWidth();
	unsigned long imageHeight = bmp->GetImageHeight();

	int index = 0;
	for (unsigned long y = 0; y < imageHeight; y++)
	{		
		for (unsigned long x = 0; x < imageWidth; x++)
		{
			unsigned char r = unsigned char(data[index++]);
			unsigned char g = unsigned char(data[index++]);
			unsigned char b = unsigned char(data[index++]);
			//std::cout << "[" << int(r) << "," << int(g) << "," << int(b) << "] ";
			std::string tempColor = GetColourCharacter(r,g,b);
			bmp->rgbVector.push_back(tempColor);
			//std::cout << tempColor << " ";
			if(tempColor == "grn"){ bmp->start = {x,y}; }
			if(tempColor == "red"){ bmp->resource = {x,y}; }
			if(tempColor == "blu"){ bmp->finish = {x,y}; }
		}
		//std::cout<<std::endl;
	}
	return bmp;
}
