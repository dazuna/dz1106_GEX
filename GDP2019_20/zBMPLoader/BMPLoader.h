#pragma once
#include <vector>
#include <string>
#include "BMPImage.h"
#include "ResourceManager.h"

class BMPLoader
{
public:
	BMPImage* bmp;
	
	std::string GetColourCharacter(
		unsigned char r, 
		unsigned char g, 
		unsigned char b);
	BMPImage* createColorVector();
};
