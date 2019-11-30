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

extern cFlyCamera* g_pFlyCamera;

int KillAllHumans(lua_State* L);
int MoveTo(lua_State* L);
int Follow(lua_State* L);
int Curve(lua_State* L);
int Rotate(lua_State* L);
int Orient(lua_State* L);
int Trigger(lua_State* L);

