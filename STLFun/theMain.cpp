#include <iostream>
//#pragma warning(disable:4789)
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <fstream>

#include "cPerson.h"

void LoadFileAndMakePeople(int numPeople, std::vector<cPerson>& vecPeople);

// Binary predicate function for sort (takes 2 things)
bool IsPersonA_GT_PersonB(cPerson& A, cPerson& B)
{
	int ALength = A.firstName.length() + A.lastName.length();
	int BLength = B.firstName.length() + B.lastName.length();

	if (ALength > BLength)
	{
		return true;	// A is greater than B
	}
	return false;		// B is greater than A
}

class cIsPersonA_GT_PersonB_Functor
{
public:
	// It only knows how to call "operator()", so you need to name your function (method) that
	bool operator() (cPerson& A, cPerson& B)
	{
		int ALength = A.firstName.length() + A.lastName.length();
		int BLength = B.firstName.length() + B.lastName.length();

		if (ALength > BLength)
		{
			return true;	// A is greater than B
		}
		return false;		// B is greater than A
	}
};

// Uniary predicate function (takes 1 thing) for for_each (and others)
void SetShoeSizeTo16(cPerson& A)
{
	A.shoeSize = 16;
	return;
}

//int x, y, z; 
//z = x + y;
//z = x - y;
//z = x / y;
//cPerson bob;
//bob(???)


class cSetShoeSize
{
public:
	cSetShoeSize() 
	{
		this->currentShoeSize = 7;
	}
	void SetTheStartingShoeSize(int startSize)
	{
		this->currentShoeSize = startSize;
	}
	int currentShoeSize;
	void SetTheShoeSize(cPerson& A);
//	void operator++();
//	void operator+();
	void operator() (cPerson& A)
	{
		A.shoeSize = this->currentShoeSize;
		this->currentShoeSize++;
	}
};

void PrintVectorOfPeople( std::vector<cPerson>& vecPeople )
{
	//for (std::vector<int>::iterator itVec = myVec.begin();
	//	 itVec != myVec.end();		// 1 past the end of the vector
	//	 itVec++)
	for (std::vector<cPerson>::iterator itPerson = vecPeople.begin();
		 itPerson != vecPeople.end();
		 itPerson++)
	{
//		std::cout << (*itPerson).firstName << std::endl;
		std::cout 
			<< itPerson->firstName << " " 
			<< itPerson->lastName << "   "
			<< itPerson->shoeSize << std::endl;
	}


}



// Behaving like an "iterator"
int getValueAtIndex(std::vector<int>& myVec, int index)
{
	return myVec[index];
}

void printVector(std::vector<int>& myVec)
{
	//for (int index = 0; index != myVec.size(); index++)
	//{
	//	std::cout << index << " : " << getValueAtIndex(myVec, index) << std::endl;
	//}
	int index = 0;
	for (std::vector<int>::iterator itVec = myVec.begin();
		 itVec != myVec.end();		// 1 past the end of the vector
		 itVec++, index++ )
	{
		std::cout << index << " : "  << *itVec << std::endl;
	}
}

void printMap(std::map<std::string, std::string>& myMap)
{
	int index = 0;
	for (std::map<std::string, std::string>::iterator itMap = myMap.begin();
		 itMap != myMap.end();		// 1 past the end of the vector
		 itMap++, index++)
	{
		std::cout << index << " : " 
			<< itMap->first << ", " 
			<< itMap->second << std::endl;
	}
}

std::vector<cPerson>::iterator
FindThisPerson( std::vector<cPerson> &vecPeople,
					    std::string nameToFind )
{
	// Find that name
	std::vector<cPerson>::iterator itPerson = vecPeople.begin();
	for ( ;
		 itPerson != vecPeople.end();
		 itPerson++)
	{
		if (itPerson->firstName == nameToFind)
		{
			return itPerson;
		}
	}
	return itPerson;
}


int main()
{
	std::vector<cPerson> vecPeople;
	LoadFileAndMakePeople(10, vecPeople);
	PrintVectorOfPeople(vecPeople);

	// Find a particular person
	std::cout << "Type a last name to find: ";
	std::string nameToFind;
	std::cin >> nameToFind;

	std::vector<cPerson>::iterator itY = FindThisPerson(vecPeople, nameToFind);

	if (itY == vecPeople.end())
	{
		std::cout << "Didn't find it" << std::endl;
	}
	else
	{
		itY->firstName = "Beyonce";
	}


	itY--;
	std::cout << itY->firstName << std::endl;

	itY++;  itY++;
	std::cout << itY->firstName << std::endl;

	vecPeople.erase( itY );



	std::map<std::string /*name*/, std::string /*food*/> mapNameToFood;
	mapNameToFood["Felipe"] = "Hay";
	mapNameToFood["Hamza"] = "Pancake";
	mapNameToFood["Dylan"] = "Fish Food";
	mapNameToFood["Ethan"] = "Fish";
	mapNameToFood["Brandon"] = "Pizza";
	mapNameToFood["Brian"] = "Dog food";
	mapNameToFood["Caleb"] = "Eggs";
	mapNameToFood["Christopher"] = "Pie";
	mapNameToFood["David"] = "Cat food";
	mapNameToFood["Dhilip"] = "Noodles";
	mapNameToFood["Harshil"] = "Chocolate";

	//std::cout << mapNameToFood["Michael"];

	std::map<std::string, std::string>::iterator itX = mapNameToFood.find("Ethan");

	if (itX == mapNameToFood.end())
	{
		std::cout << "Didn't find them" << std::endl;
	}
	else   // itX != mapNameToFood.end()
	{
		std::cout << itX->first << ", " << itX->second << std::endl;
	}

	///
	printMap(mapNameToFood);









//	std::sort( vecPeople.begin(), vecPeople.end(), IsPersonA_GT_PersonB );
	// Or with a functor...
	cIsPersonA_GT_PersonB_Functor comparitorThingy;
	std::sort( vecPeople.begin(), vecPeople.end(), comparitorThingy);
//	std::sort( vecPeople.begin(), vecPeople.end(), cIsPersonA_GT_PersonB_Functor() );

	std::for_each( vecPeople.begin(), vecPeople.end(), SetShoeSizeTo16 );

	// Functor ( predicate function class)
	cSetShoeSize shoeSizeSetter;
	shoeSizeSetter.SetTheStartingShoeSize(25);
	std::for_each( vecPeople.begin(), vecPeople.end(), shoeSizeSetter );

	std::cout << std::endl;

	PrintVectorOfPeople(vecPeople);

	//int x[10] = {0};		// 0-9			// At compile time (STACK)
	//int* pX = new int[10];		// At run time (HEAP)
	//x[3] = 123123;
	//pX[3] = 1323321;

	std::vector<int> myVec;	// 0
	myVec.push_back(48);
	myVec.push_back(3);
	myVec.push_back(76);
	myVec.push_back(13);
	myVec.push_back(45);
	myVec.push_back(63);
	myVec.push_back(7);

	printVector(myVec);

	std::sort( myVec.begin(), myVec.end() );


	printVector(myVec);











	return 0;
}