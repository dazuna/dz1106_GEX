#include "GameCursor.h"
#include "util/tools.h"
#include "Terrain.h"
#include "GameTools.h"

int GameCursor::coord_x = -1, GameCursor::coord_y = -1;
cGameObject* GameCursor::cursorObj = nullptr;
cGameObject* GameCursor::cursorSquare = nullptr;

void GameCursor::init()
{
	if (!tools::pFindObjectByFriendlyNameMap("cursor"))
	{
		std::cout << "No cursor object!" << std::endl;
		return;
	}

	cursorObj = ::g_map_GameObjects.at("cursor");
	cursorSquare = ::g_map_GameObjects.at("squareCursor");
}

void GameCursor::moveCursor(int dir_x, int dir_y)
{
	int new_x = coord_x + dir_x;
	int new_y = coord_y + dir_y;
	if (GameTools::validCoord(new_x, new_y))
	{
		coord_x = new_x; coord_y = new_y;
		auto newWorldPos = GameTools::coordToWorldPos(new_x, new_y);
		newWorldPos.y = cursorObj->positionXYZ.y;
		cursorObj->positionXYZ = newWorldPos;
		newWorldPos.y = cursorSquare->positionXYZ.y;		
		cursorSquare->positionXYZ = newWorldPos;
	}
}

void GameCursor::setCoordinates(int new_x, int new_y)
{
	if (GameTools::validCoord(new_x, new_y))
	{
		coord_x = new_x; coord_y = new_y;
		auto newWorldPos = GameTools::coordToWorldPos(new_x, new_y);
		newWorldPos.y = cursorObj->positionXYZ.y;
		cursorObj->positionXYZ = newWorldPos;
		newWorldPos.y = cursorSquare->positionXYZ.y;		
		cursorSquare->positionXYZ = newWorldPos;
	}
}
