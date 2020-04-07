#include "GameEvents.h"

#include "GameTools.h"

GameEvents::vGameArmies GameEvents::allyStates;
GameEvents::vGameArmies GameEvents::enemyStates;

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

void GameEvents::goBackInTime(int stepsBack)
{
	if(allyStates.empty()){return;}
	auto idxUnit = 0;
	GameArmies::allyUnits.clear();
	const auto idxEvent = int(allyStates.size()) - stepsBack;
	for(idxUnit = 0; idxUnit < allyStates[idxEvent].size(); idxUnit++)
	{
		auto tmpAlly = new GameUnit(allyStates[idxEvent][idxUnit]);
		tmpAlly->gameObj->positionXYZ = GameTools::coordToWorldPos(tmpAlly->coord_x, tmpAlly->coord_y);
		tmpAlly->gameObj->isVisible = true;
		GameArmies::allyUnits.push_back(tmpAlly);
	}
	GameArmies::enemyUnits.clear();
	for(idxUnit = 0; idxUnit < enemyStates[idxEvent].size(); idxUnit++)
	{
		auto tmpEnemy = new GameUnit(enemyStates[idxEvent][idxUnit]);
		tmpEnemy->gameObj->positionXYZ = GameTools::coordToWorldPos(tmpEnemy->coord_x, tmpEnemy->coord_y);
		tmpEnemy->gameObj->isVisible = true;
		GameArmies::enemyUnits.push_back(tmpEnemy);
	}
	GameArmies::selectedAlly = GameArmies::allyUnits.begin();
	for(auto i = 0; i < stepsBack; i++)
	{
		allyStates.pop_back();
		enemyStates.pop_back();
	}
	std::cout<<"time was reversed successfully"<<std::endl;	
}

