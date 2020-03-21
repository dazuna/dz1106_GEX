#include "JsonState.h"

JsonState* JsonState::theJsonState = new JsonState();

JsonState* JsonState::getTheJsonState()
{
	return theJsonState;
}

int JsonState::findObject(cGameObject* theGO)
{
	auto index = 0;
	for(index; index < JSONObjects.size(); index++)
	{
		if(JSONObjects[index]["friendlyName"] == theGO->friendlyName)
		{
			return index;
		}
	}
	return -1;
}

void JsonState::mergeObject(cGameObject* theGO)
{
	auto index = findObject(theGO);
	json jsonObject;	
	jsonObject["positionXYZ"][0] = theGO->positionXYZ.x;
	jsonObject["positionXYZ"][1] = theGO->positionXYZ.y;
	jsonObject["positionXYZ"][2] = theGO->positionXYZ.z;
	jsonObject["rotationXYZ"][0] = theGO->getEulerAngle().x;
	jsonObject["rotationXYZ"][1] = theGO->getEulerAngle().y;
	jsonObject["rotationXYZ"][2] = theGO->getEulerAngle().z;
	jsonObject["scale"] = theGO->scale;
	jsonObject["isVisible"] = theGO->isVisible;
	jsonObject["alphaTransparency"] = theGO->alphaTransparency;

	if(index>=0)
	{
		JSONObjects[index].merge_patch(jsonObject);
	}
}

void JsonState::mergeAllObjects(const mapGO& GOsMap)
{
	for(auto game_object : GOsMap)
	{
		mergeObject(game_object.second);
	}
}

int JsonState::findLight(cLight* theLight)
{
	auto index = 0;
	for(index; index < JSONObjects.size(); index++)
	{
		if(JSONObjects[index]["name"] == theLight->name)
		{
			return index;
		}
	}
	return -1;
}

void JsonState::mergeLight(cLight* theLight)
{
	auto index = findLight(theLight);
	json jsonObject;
	jsonObject["positionXYZ"][0] = theLight->positionXYZ.x;
	jsonObject["positionXYZ"][1] = theLight->positionXYZ.y;
	jsonObject["positionXYZ"][2] = theLight->positionXYZ.z;
	jsonObject["LinearAtten"] = theLight->LinearAtten;
	jsonObject["QuadraticAtten"] = theLight->QuadraticAtten;
	jsonObject["direction"][0] = theLight->getCurrentAT().x;
	jsonObject["direction"][1] = theLight->getCurrentAT().y;
	jsonObject["direction"][2] = theLight->getCurrentAT().z;
	jsonObject["innerAngle"] = theLight->innerAngle;
	jsonObject["outerAngle"] = theLight->outerAngle;
	jsonObject["lightSwitch"] = theLight->lightSwitch;

	if(index>=0)
	{
		JSONLights[index].merge_patch(jsonObject);
	}
}

void JsonState::mergeAllLights(const mapLights& lightMap)
{
	for(auto game_object : lightMap)
	{
		mergeLight(game_object.second);
	}
}

