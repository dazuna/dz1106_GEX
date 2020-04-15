#include "GameEvents.h"

#include "GameTools.h"
#include "util/tools.h"

GameEvents::vGameArmies GameEvents::allyStates;
GameEvents::vGameArmies GameEvents::enemyStates;
std::string GameEvents::effectState = "inactive";
float GameEvents::effectTimer = 0.f;
int GameEvents::stepsBack = 0;

void GameEvents::saveAllies()
{
	vecGameUnit allyArmyState;
	for (auto pAllyUnit : GameArmies::allyUnits)
	{
		GameUnit allyUnit = *pAllyUnit;
		if(allyUnit.state == "walking") {allyUnit.state = "waiting";}
		allyArmyState.push_back(allyUnit);
	}
	allyStates.push_back(allyArmyState);
}

void GameEvents::saveEnemies()
{
	vecGameUnit enemyArmyState;
	for (auto pEnemyUnit : GameArmies::enemyUnits)
	{
		GameUnit enemyUnit = *pEnemyUnit;
		enemyArmyState.push_back(enemyUnit);
	}
	enemyStates.push_back(enemyArmyState);
}

void GameEvents::saveGameState()
{
	saveAllies();
	saveEnemies();
	std::cout << "saved game state!" << std::endl;
}

void GameEvents::update(float dt)
{
	if (!tools::pFindObjectByFriendlyNameMap("zawarudoSphere")) return;

	auto zawarudoSphere = ::g_map_GameObjects["zawarudoSphere"];
	if (effectState == "growing")
	{
		zawarudoSphere->scale *= 1.3f;

		if (zawarudoSphere->scale > 400)
		{
			effectState = "active";
		}
	}
	
	if (effectState == "shrinking")
	{
		zawarudoSphere->scale *= 0.7f;

		if (zawarudoSphere->scale < 1)
		{
			zawarudoSphere->scale = 0.f;
			effectState = "inactive";
		}
	}
}

void GameEvents::inputListen(int key, int action, int mods)
{
	if (!tools::pFindObjectByFriendlyNameMap("zawarudoSphere")) return;
	auto zawarudoSphere = ::g_map_GameObjects["zawarudoSphere"];

	auto selectedUnit = (*GameArmies::selectedAlly);
	if (!(selectedUnit->state == "waiting" || selectedUnit->state == "inactive"))
		return;
	
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (effectState == "inactive")
		{
			zawarudoSphere->scale = 1.f;
			effectState = "growing";
			zawarudoSphere->positionXYZ = (*GameArmies::selectedAlly)->gameObj->positionXYZ;
			saveGameState();
		}

		if (effectState == "active")
		{
			effectState = "shrinking";
			commitState();
		}
	}

	if (effectState == "active")
	{
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			if (stepsBack < allyStates.size() - 1) stepsBack++;
			goBackInTime(stepsBack);
		}

		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			if (stepsBack > 0) stepsBack--;
			goBackInTime(stepsBack);
		}
	}
}

void GameEvents::commitState()
{
	if (allyStates.empty()) return;
	// we delete the number of movements we went back, plus the state
	// we end up with to remain consistent
	for (auto i = 0; i < stepsBack + 1; i++)
	{
		allyStates.pop_back();
		enemyStates.pop_back();
	}
	stepsBack = 0;
}

void GameEvents::deleteHistory()
{
	allyStates.clear();
	enemyStates.clear();
	stepsBack = 0;
}

void restoreArmy(vUnits& army, const GameEvents::vecGameUnit& state)
{
	for (auto& unit : army)
	{
		unit->gameObj->isVisible = false;
	}
	army.clear();
	
	for(const auto& unitState : state)
	{
		auto tmpAlly = new GameUnit(unitState);
		tmpAlly->gameObj->positionXYZ = GameTools::coordToWorldPos(tmpAlly->coord_x, tmpAlly->coord_y);
		tmpAlly->gameObj->isVisible = true;
		army.push_back(tmpAlly);
	}
}

void GameEvents::goBackInTime(int stepsBack)
{
	if (stepsBack < 0 || stepsBack >= allyStates.size() || allyStates.empty()) return;
	auto idxUnit = 0;
	GameArmies::deselectUnit(*GameArmies::selectedAlly);

	// assuming original state is in last position
	const auto idxEvent = int(allyStates.size()) - stepsBack - 1;
	restoreArmy(GameArmies::allyUnits, allyStates[idxEvent]);
	restoreArmy(GameArmies::enemyUnits, enemyStates[idxEvent]);
	
	GameArmies::selectedAlly = GameArmies::allyUnits.begin();
	GameArmies::selectUnit(GameArmies::allyUnits.begin());
	
	std::cout<<"time was reversed successfully"<<std::endl;	
}

