#include "cPerson.h"
#include <vector>
#include <fstream>
#include <iostream>


std::string MakeNamePretty(std::string name)
{
	// TODO:

	return name;
}


// returns number of names laoded
unsigned int LoadNameFile(std::string fileName, std::vector<std::string>& vecNames)
{
	unsigned int nameCount = 0;

	std::ifstream theFile(fileName.c_str());
	if ( ! theFile.is_open())
	{
		return nameCount;
	}

	std::string name;
	while (theFile >> name)
	{
		// SMITH 1.006 1.006 1
		// Load the other three numbers
		float discard;
		theFile >> discard >> discard >> discard;

		// Make name pretty
		// 1st letter is capitol
		// rest is lower case
		name = MakeNamePretty(name);

		vecNames.push_back(name);
		nameCount++;
	}

	return nameCount;
}


bool bFilesLoaded = false;
std::vector<std::string> vecBoyNames;
std::vector<std::string> vecGirlNames;
std::vector<std::string> vecLastNames;

std::string pickRandomNameFromVector(std::vector<std::string>& vecNames)
{
	int index = rand() % ((int)vecNames.size() - 1);

	return vecNames[index];
}

void LoadFileAndMakePeople(int numPeople, std::vector<cPerson>& vecPeople)
{
	//	int index = rand() % (myVec.size() - 1);
	//	myVec[index];
		// TODO:
		// Open the first name files
		// Open the last name file
		// Pick a random gender (boy or girl)
		// Pick a random last name
		// Add that person to the vector

	if ( ! bFilesLoaded )
	{
		unsigned int numberOfNamesLoaded = 0;
	
		numberOfNamesLoaded = LoadNameFile("dist.female.first.txt", vecGirlNames);
		std::cout << numberOfNamesLoaded << " girls names loaded" << std::endl;

		numberOfNamesLoaded = LoadNameFile("dist.male.first.txt", vecBoyNames);
		std::cout << numberOfNamesLoaded << " boys names loaded" << std::endl;

		numberOfNamesLoaded = LoadNameFile("US_LastNames.txt", vecLastNames);
		std::cout << numberOfNamesLoaded << " last names loaded" << std::endl;

		bFilesLoaded = true;
	}//if ( ! bFilesLoaded )

	for (int count = 0; count != numPeople; count++)
	{
		cPerson aPerson;

		// Boy or Girl?
		if (rand() % 100 > 50)
		{	// Girl
			aPerson.firstName = pickRandomNameFromVector(vecGirlNames);
		}
		else
		{	// Boy
			aPerson.firstName = pickRandomNameFromVector(vecBoyNames);
		}
		aPerson.lastName = pickRandomNameFromVector(vecLastNames);

		aPerson.shoeSize = 3 + (rand() % 13);

		vecPeople.push_back(aPerson);
	}//for (int count = 0

	return;
}

//void LoadFileAndMakePeople(int numPeople, std::vector<cPerson>& vecPeople)
//{
//	//	int index = rand() % (myVec.size() - 1);
//	//	myVec[index];
//		// TODO:
//		// Open the first name files
//		// Open the last name file
//		// Pick a random gender (boy or girl)
//		// Pick a random last name
//		// Add that person to the vector
//	vecPeople.push_back(cPerson("Johnny", "Depp"));
//	vecPeople.push_back(cPerson("Sweeney", "Todd"));
//	vecPeople.push_back(cPerson("Arnold", "Schwarzenegger"));
//	vecPeople.push_back(cPerson("Jim", "Carrey"));
//	vecPeople.push_back(cPerson("Emma", "Watson"));
//	vecPeople.push_back(cPerson("Michael", "Feeney"));
//
//	return;
//}
