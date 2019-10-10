#pragma once

#include <string>

class cPerson
{
public:
	cPerson() {};
	cPerson(std::string first, std::string last)
	{
		this->firstName = first;
		this->lastName = last;
		this->shoeSize = -1;	// UNKNOWN
	}
	std::string firstName;
	std::string lastName;
	int shoeSize;
};