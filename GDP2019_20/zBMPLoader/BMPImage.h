#ifndef BMPImage_HG
#define BMPImage_HG

#include <string>
#include <vector>

class BMPImage
{
public:
	BMPImage(const std::string &filename);
	virtual ~BMPImage(void);

	bool IsLoaded(void);
	unsigned long GetFileSize(void);
	unsigned long GetImageWidth(void);
	unsigned long GetImageHeight(void);
	unsigned long GetImageSize(void);
	char* GetData(void);

	BMPImage(void);
	bool LoadBMPFromFile(const std::string &filename);

	bool m_bIsLoaded;
	unsigned long m_FileSize;
	unsigned long m_ImageWidth;
	unsigned long m_ImageHeight;
	unsigned long m_ImageSize;
	char* m_pData;
	std::vector<std::string> rgbVector;
	std::pair<int,int> start;
	std::pair<int,int> resource;
	std::pair<int,int> finish;
};

#endif
