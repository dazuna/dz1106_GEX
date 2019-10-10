#include "cLASER.h"
#include <iostream>

void cLASER::Fire(void)
{	
	std::cout << "cLASER says: Zap!" << std::endl;
}

void cLASER::SetSize(float newSize)
{
	this->size = newSize;
}
float cLASER::GetSize(void)
{
	return this->size;
}
