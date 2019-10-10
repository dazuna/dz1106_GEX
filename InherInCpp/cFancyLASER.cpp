#include "cFancyLASER.h"
#include <iostream>

void cFancyLASER::Fire(void)
{
	std::cout << "cFancyLASER says: Zap!" << std::endl;
}

void cFancyLASER::SetSize(float newSize)
{
	this->size = newSize;
}
float cFancyLASER::GetSize(void)
{
	return this->size;
}