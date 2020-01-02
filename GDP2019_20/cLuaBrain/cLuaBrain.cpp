#include "cLuaBrain.h"
#include "cLuaToCommand.hpp"
#include <iostream>

const unsigned int MAXLINELENGTH = 65536;
std::map<std::string, iCommand*> cmdDictionary;
iCommand* masterCommandGroup = NULL;

cLuaBrain::cLuaBrain()
{

	// Create new Lua state.
	// NOTE: this is common to ALL script in this case
	this->m_pLuaState = luaL_newstate();

	luaL_openlibs(this->m_pLuaState);					/* Lua 5.3.3 */

	lua_pushcfunction(this->m_pLuaState, KillAllHumans);
	lua_setglobal(this->m_pLuaState, "KillAllHumans");
	lua_pushcfunction(this->m_pLuaState,MoveTo);
	lua_setglobal(this->m_pLuaState, "MoveTo");
	lua_pushcfunction(this->m_pLuaState,Follow);
	lua_setglobal(this->m_pLuaState, "Follow");
	lua_pushcfunction(this->m_pLuaState,Curve);
	lua_setglobal(this->m_pLuaState, "Curve");
	lua_pushcfunction(this->m_pLuaState,Rotate);
	lua_setglobal(this->m_pLuaState, "Rotate");
	lua_pushcfunction(this->m_pLuaState,Orient);
	lua_setglobal(this->m_pLuaState, "Orient");
	lua_pushcfunction(this->m_pLuaState,Trigger);
	lua_setglobal(this->m_pLuaState, "Trigger");
	
	lua_pushcfunction(this->m_pLuaState,CamLookAt);
	lua_setglobal(this->m_pLuaState, "CamLookAt");

	lua_pushcfunction(this->m_pLuaState, MoveLightTo);
	lua_setglobal(this->m_pLuaState, "MoveLightTo");
	lua_pushcfunction(this->m_pLuaState, FollowLight);
	lua_setglobal(this->m_pLuaState, "FollowLight");
	lua_pushcfunction(this->m_pLuaState, RotateLight);
	lua_setglobal(this->m_pLuaState, "RotateLight");

	lua_pushcfunction(this->m_pLuaState, newSerial);
	lua_setglobal(this->m_pLuaState, "newSerial");
	lua_pushcfunction(this->m_pLuaState, newParallel);
	lua_setglobal(this->m_pLuaState, "newParallel");
	lua_pushcfunction(this->m_pLuaState, addToSerial);
	lua_setglobal(this->m_pLuaState, "addToSerial");
	lua_pushcfunction(this->m_pLuaState, addToParallel);
	lua_setglobal(this->m_pLuaState, "addToParallel");
	lua_pushcfunction(this->m_pLuaState, setTheCommandMasterToRuleThemAll);
	lua_setglobal(this->m_pLuaState, "setTheCommandMasterToRuleThemAll");


	return;
}

cLuaBrain::~cLuaBrain()
{
	lua_close(this->m_pLuaState);
	return;
}

bool cLuaBrain::LoadScript( std::string scriptName )
{
	std::cout << "loading script...";
	std::ifstream theFile(scriptName.c_str());
	if (!theFile.is_open())
	{
		return false;
	}

	char pLineTemp[MAXLINELENGTH] = { 0 };
	std::string tempString2 = "";
	while (theFile.getline(pLineTemp, MAXLINELENGTH))
	{
		std::string tempString(pLineTemp);
		tempString += "\n";
		tempString2 += tempString;
	}
	theFile.close();

	//std::cout << tempString2 << std::endl;
	int error = luaL_loadstring(this->m_pLuaState, tempString2.c_str());

	if (error != 0 /*no error*/)
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		return false;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
		0,	/* nargs: number of arguments pushed onto the lua stack */
		0,	/* nresults: number of results that should be on stack at end*/
		0);	/* errfunc: location, in stack, of error function.
				if 0, results are on top of stack. */

	if (error != 0 /*no error*/)
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append(lua_tostring(this->m_pLuaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */
	}
	return true;
}

void cLuaBrain::DeleteScript( std::string scriptName )
{
	this->m_mapScripts.erase(scriptName);
	return;
}

void cLuaBrain::Update(float deltaTime)
{
	if(masterCommandGroup)
		masterCommandGroup->Update(deltaTime);
	return;
}

void cLuaBrain::RunScriptImmediately(std::string script)
{

	int error = luaL_loadstring( this->m_pLuaState, 
								 script.c_str() );

	if ( error != 0 /*no error*/)	
	{
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		return;
	}

	// execute funtion in "protected mode", where problems are 
	//  caught and placed on the stack for investigation
	error = lua_pcall(this->m_pLuaState,	/* lua state */
						0,	/* nargs: number of arguments pushed onto the lua stack */
						0,	/* nresults: number of results that should be on stack at end*/
						0);	/* errfunc: location, in stack, of error function. 
								if 0, results are on top of stack. */
	if ( error != 0 /*no error*/)	
	{
		std::cout << "Lua: There was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from top of stack (-1 is top)
		luaError.append( lua_tostring(this->m_pLuaState, -1) );

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
		// We passed zero (0) as errfunc, so error is on stack)
		lua_pop(this->m_pLuaState, 1);  /* pop error message from the stack */
	}

	return;
}

std::string cLuaBrain::m_decodeLuaErrorToString( int error )
{
	switch ( error )
	{
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error"; 
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	}//switch ( error )

	// Who knows what this error is?
	return "Lua: UNKNOWN error";
}

