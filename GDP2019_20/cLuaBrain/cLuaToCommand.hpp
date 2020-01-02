#pragma once

extern "C" 
{
	#include <Lua/lua.h>
	#include <Lua/lauxlib.h>
	#include <Lua/lualib.h>
}

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "../globalStuff.h"
#include "../cFlyCamera/cFlyCamera.h"
#include "../cCommands/iCommand.h"
#include "../cCommands/cMoveTo_AB_Time.h"
#include "../cCommands/cCommandGroupSerial.h"
#include "../cCommands/cCommandGroupParallel.h"
#include "../cCommands/cRotateTo_Time.hpp"
#include "../cCommands/cOrientTo_Time.hpp"
#include "../cCommands/cFollowObject.hpp"
#include "../cCommands/cBezierCurve.hpp"
#include "../cCommands/cTrigger.hpp"
#include "../cCommands/cRotateTo_Light.hpp"
#include "../cCommands/cCamLookAt.h"

extern cFlyCamera* g_pFlyCamera;
extern std::map<std::string, iCommand*> cmdDictionary;
extern iCommand* masterCommandGroup;

int KillAllHumans(lua_State* L);
int MoveTo(lua_State* L);
int Follow(lua_State* L);
int Curve(lua_State* L);
int Rotate(lua_State* L);
int Orient(lua_State* L);
int Trigger(lua_State* L);

int CamLookAt(lua_State* L);

int MoveLightTo(lua_State* L);
int FollowLight(lua_State* L);
int RotateLight(lua_State* L);

int newSerial(lua_State* L);
int newParallel(lua_State* L);
int addToSerial(lua_State* L);
int addToParallel(lua_State* L);
int setTheCommandMasterToRuleThemAll(lua_State* L);


//cGameObject* pSphere = ::g_map_GameObjects["sphere"];
//cGameObject* pSphere2 = ::g_map_GameObjects["sphere2"];
//cGameObject* pTie = ::g_map_GameObjects["cube1"];
//
//cMoveTo_AB_Time* moveTo = new cMoveTo_AB_Time(pSphere, pSphere->positionXYZ, glm::vec3(20, 20, 0), 20.0f, 2.0f, 2.0f);
//cMoveTo_AB_Time* moveTo2 = new cMoveTo_AB_Time(pSphere2, pSphere2->positionXYZ, glm::vec3(-20, -20, 0), 10.0f, 2.0f, 2.0f);
//cRotateTo_Time* rollTo = new cRotateTo_Time("roll1", "roll1", pTie, glm::vec3(90, 0, 0), 5.0f);
//cRotateTo_Time* rollTo = new cRotateTo_Time("roll1", "roll1", pTie, glm::vec3(0, 170, 0), 10.0f, 3, 3);
//cRotateTo_Time* rollTo2 = new cRotateTo_Time("roll2", "roll2", pTie, glm::vec3(0, 90, 0), 5.0f);
//cOrientTo_Time* oriTo = new cOrientTo_Time("ori1", "ori1", pTie, glm::vec3(1,0,1), 5.0f);
//cFollowObject* follow = new cFollowObject("foll1", "foll1", &(pSphere->positionXYZ), &(pTie->positionXYZ), 50.0f, glm::vec3(0, 5, -2), 10.0f, 1.0f, 20.0f);
//cFollowObject* follow = new cFollowObject("foll1", "foll1", &(pSphere2->positionXYZ), &(pSphere->positionXYZ), 50.0f, glm::vec3(0, 5, -20), 10.0f, 1.0f, 20.0f);
//cBezierCurve* bezier = new cBezierCurve("bez1", "bez1", &(pSphere2->positionXYZ), pSphere2->positionXYZ, glm::vec3(8, 30, 0), glm::vec3(0, 20, 5), 6.0f, 2.0f, 2.0f);
//cTrigger* triggs = new cTrigger("trig1", "trig1", &(pTie->positionXYZ), glm::vec3(30, 0, 0), rollTo, 5.0f, 60.0f);
//cCommandGroupSerial* CGSerial = new cCommandGroupSerial("thoseMoves", "movingCoolSerial");
//cCommandGroupParallel* CGParallel = new cCommandGroupParallel("thoseMoves", "movingCoolParallel");
//
//CGSerial->AddCommandSerial(moveTo);
//CGSerial->AddCommandSerial(moveTo2);
//CGSerial->AddCommandSerial(rollTo);
//CGSerial->AddCommandSerial(oriTo);
//CGSerial->AddCommandSerial(triggs);
//
//CGParallel->AddCommandParallel(moveTo);
//CGParallel->AddCommandParallel(moveTo2);
//CGParallel->AddCommandParallel(rollTo);
//CGParallel->AddCommandParallel(follow);
//CGParallel->AddCommandParallel(bezier);
