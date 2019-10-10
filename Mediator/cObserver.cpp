#include "cObserver.h"
#include <iostream>

void cObserver::setRobot(cRobot* pTheRobot)
{
	this->pTheRobot = pTheRobot;
}

void cObserver::Update(void)
{
	std::cout << "Objerver: " << this->pTheRobot->energy << std::endl;
}


