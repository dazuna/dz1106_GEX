#include "cBigFreakingLASER.h"

void cBigFreakingLASER::Fire(void)
{
	std::cout << "Big LASER: ZAAAAP!" << std::endl;
}

void cBigFreakingLASER::SetSize(float newSize)
{
	this->size = newSize;
}
float cBigFreakingLASER::GetSize(void)
{
	return this->size;
}
