#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

extern "C" {
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "../cCommands/iCommand.h"
#include "../cCommands/cMoveTo_AB_Time.h"
#include "../cCommands/cCommandGroupSerial.h"
#include "../cCommands/cCommandGroupParallel.h"
#include "../cCommands/cRotateTo_Time.hpp"
#include "../cCommands/cOrientTo_Time.hpp"
#include "../cCommands/cFollowObject.hpp"
#include "../cCommands/cBezierCurve.hpp"
#include "../cCommands/cTrigger.hpp"

// Forward declaration for cyclical reference
class cGameObject;

class cLuaBrain
{
public:
	// Init Lua and set callback functions
	cLuaBrain();
	~cLuaBrain();
	bool LoadScript( std::string scriptName );
	void DeleteScript( std::string scriptName );
	void Update(float deltaTime);

	void RunScriptImmediately(std::string script);

private:
	std::map< std::string, iCommand*> m_mapScripts;
	lua_State* m_pLuaState;
	std::string m_decodeLuaErrorToString( int error );
};

#endif
