#pragma once
#include <nlohmann/json.hpp>
#include "definitions.h"
#include "cGameObject.h"
#include "cLight.h"

using json = nlohmann::json;

struct JsonState
{
	static JsonState* theJsonState;
	static JsonState* getTheJsonState();

	json JSONObjects;
	json JSONLights;

	int findObject(cGameObject* theGO);
	void mergeObject(cGameObject* theGO);
	void mergeAllObjects(const mapGO& GOsMap);
	int findLight(cLight* theLight);
	void mergeLight(cLight* theLight);
	void mergeAllLights(const mapLights& lightMap);
};

