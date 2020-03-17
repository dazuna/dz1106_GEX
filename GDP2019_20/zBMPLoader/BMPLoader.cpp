#include "BMPImage.h"
#include "ResourceManager.h"

ResourceManager gResourceManager;

char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255	&& g == 0	&& b == 0)		return 'r';
	if (r == 0		&& g == 255 && b == 0)		return 'g';
	if (r == 0		&& g == 0	&& b == 255)	return 'b';
	if (r == 255	&& g == 255 && b == 255)	return 'w';
	if (r == 0		&& g == 0	&& b == 0)		return '_';
	return 'x';
}

int main(int argc, char** argv)
{
	BMPImage* bmp = new BMPImage("example.bmp");

	char* data = bmp->GetData();
	unsigned long imageWidth = bmp->GetImageWidth();
	unsigned long imageHeight = bmp->GetImageHeight();

	int index = 0;
	unsigned short r, g, b;
	for (unsigned long x = 0; x < imageWidth; x++) {
		for (unsigned long y = 0; y < imageHeight; y++) {
			printf("%c", GetColourCharacter(data[index++], data[index++], data[index++]));
		}
		printf("\n");
	}
}