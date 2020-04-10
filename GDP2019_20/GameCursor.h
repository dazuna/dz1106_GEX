#pragma once
#include "cGameObject.h"
#include "cFlyCamera/cFlyCamera.h"

struct GameCursor
{
	static int coord_x, coord_y;
	static cGameObject *cursorObj, *cursorSquare;
	static cFlyCamera* miniMapCamera;
	static void init();
	static void moveCursor(int dir_x, int dir_y);
	static void setCoordinates(int new_x, int new_y);
	static glm::vec3 getWorldPos();
	static void repositionCamera();
};
