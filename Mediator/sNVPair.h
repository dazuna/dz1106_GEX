#ifndef _sNVPair_HG_
#define _sNVPair_HG_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct sNVPair
{
	sNVPair();
	sNVPair(std::string name_, std::string sValue_);
	sNVPair(std::string name_, float fValue_);
	sNVPair(std::string name_, int iValue_);
	sNVPair(std::string name_, glm::vec3 v3Value_);

	sNVPair(std::string sValue_);
	sNVPair(float fValue_);
	sNVPair(int iValue_);
	sNVPair(glm::vec3 v3Value_);
	//TODO:	sNVPair(std::string textToParse);
	std::string name;
	std::string sValue;
	float fValue;
	int iValue;
	glm::vec3 v3Value;
	void clear(void);

	std::vector<sNVPair> vecChildNVPairs;
};



#endif
