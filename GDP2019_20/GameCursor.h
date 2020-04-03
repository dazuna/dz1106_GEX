#pragma once
#include "cGameObject.h"

struct GameCursor
{
	static int coord_x, coord_y;
	static cGameObject* cursorObj;
	static void init();
	static void moveCursor(int dir_x, int dir_y);
	static void setCoordinates(int new_x, int new_y);
};
