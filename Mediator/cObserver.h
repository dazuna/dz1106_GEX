#pragma once
#include "cRobot.h"

class cObserver
{
public:
	void setRobot( cRobot* pTheRobot );
	void Update(void);

	cRobot* pTheRobot;
};