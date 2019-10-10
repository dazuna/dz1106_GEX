#include "sNVPair.h"


void sNVPair::clear(void)
{
	this->name = "";
	this->sValue = "";
	this->fValue = 0.0f;
	this->iValue = 0;
	this->v3Value = glm::vec3(0.0f, 0.0f, 0.0f);
	return;
}
sNVPair::sNVPair()
{
	this->clear();
	return;
}

sNVPair::sNVPair(std::string name_, std::string sValue_)
{
	this->clear();
	this->name = name_;
	this->sValue = sValue_;
	return;
}

sNVPair::sNVPair(std::string name_, float fValue_)
{
	this->clear();
	this->name = name_;
	this->fValue = fValue_;
	return;
}

sNVPair::sNVPair(std::string name_, int iValue_)
{
	this->clear();
	this->name = name_;
	this->iValue = iValue_;
	return;
}

sNVPair::sNVPair(std::string name_, glm::vec3 v3Value_)
{
	this->clear();
	this->name = name_;
	this->v3Value = v3Value_;
	return;
}


sNVPair::sNVPair(std::string sValue_)
{
	this->clear();
	this->sValue = sValue_;
	return;
}

sNVPair::sNVPair(float fValue_)
{
	this->clear();
	this->fValue = fValue_;
	return;
}

sNVPair::sNVPair(int iValue_)
{
	this->clear();
	this->iValue = iValue_;
	return;
}

sNVPair::sNVPair(glm::vec3 v3Value_)
{
	this->clear();
	this->v3Value = v3Value_;
	return;
}
