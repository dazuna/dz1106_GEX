#include "cLuaToCommand.hpp"

extern std::map<std::string, iCommand*> cmdDictionary;
//iCommand* 

int KillAllHumans(lua_State* L)
{
	// There are 3 things on the stack: "Justin", 47, 3.14159 
	const char* name = lua_tostring(L, 1);	// get "Justin"		
	int age = lua_tonumber(L, 2);			// "Cat"
	float pi = lua_tonumber(L, 3);			// 3.14159

	std::cout << "KillAllHumans(): "
		<< name << ", " << age << ", " << pi << std::endl;
	return 0;
}

int MoveTo(lua_State* L)
{
	// cMoveTo_AB_Time
	// (std::string name, std::string type, 
	// cGameObject* pGO, glm::vec3 startPosition, 
	// glm::vec3 endPosition, float TimeToMove, 
	// float easeIn, float easeOut);
	const char* name = lua_tostring(L, 1);
	const char* type = lua_tostring(L, 2);
	const char* objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 startPos = glm::vec3(start_x,start_y,start_z);
	float end_x = lua_tonumber(L, 7);
	float end_y = lua_tonumber(L, 8);
	float end_z = lua_tonumber(L, 9);
	glm::vec3 endPos = glm::vec3(end_x,end_y,end_z);
	float TimeToMove = lua_tonumber(L, 10);
	float easeIn = lua_tonumber(L, 11); 
	float easeOut = lua_tonumber(L, 12);

	iCommand* nComm = new cMoveTo_AB_Time(name,type,pGO,startPos,endPos,TimeToMove,easeIn,easeOut);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int Follow(lua_State* L)
{
	// cFollowObject(std::string name, std::string type,
	// 	glm::vec3* followerPos, glm::vec3* targetPos,
	// 	float TimeToFollow, glm::vec3 offsetTarget,
	// 	float maxDistance_MaxVelocity, float minDistance_ZeroVelocity,
	// 	float maxSpeed);
	const char* name = lua_tostring(L, 1);
	const char* type = lua_tostring(L, 2);
	const char* objt = lua_tostring(L, 3);
	cGameObject* pGO;
	const char* objt2 = lua_tostring(L, 4);
	cGameObject* pGO2 = ::g_map_GameObjects[objt2];
	float TimeToFollow = lua_tonumber(L, 5);
	float offset_x = lua_tonumber(L, 6);
	float offset_y = lua_tonumber(L, 7);
	float offset_z = lua_tonumber(L, 8);
	glm::vec3 offsetVec = glm::vec3(offset_x,offset_y,offset_z);
	float maxDist = lua_tonumber(L, 9);
	float minDist = lua_tonumber(L, 10); 
	float maxSpeed = lua_tonumber(L, 11);

	iCommand* nComm = NULL;
	if(objt != "cam")
	{
		cGameObject* pGO = ::g_map_GameObjects[objt];
		nComm = new cFollowObject(name,type,&(pGO->positionXYZ),&(pGO2->positionXYZ),
		TimeToFollow,offsetVec,maxDist,minDist,maxSpeed);
	}
	else
	{
		nComm = new cFollowObject(name,type,&(::g_pFlyCamera->eye),&(pGO2->positionXYZ),
		TimeToFollow,offsetVec,maxDist,minDist,maxSpeed);
	}
	::cmdDictionary.insert({name,nComm});	

	return 0;
}

int Curve(lua_State* L)
{
	// cBezierCurve(std::string name, std::string type,
	// 	glm::vec3* itemToMove,
	// 	glm::vec3 startPos, glm::vec3 endPos,
	// 	glm::vec3 ctlPos, float timeToMove,
	// 	float easeIn, float easeOut);
	const char* name = lua_tostring(L, 1);
	const char* type = lua_tostring(L, 2);
	const char* objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 startPos = glm::vec3(start_x,start_y,start_z);
	float end_x = lua_tonumber(L, 7);
	float end_y = lua_tonumber(L, 8);
	float end_z = lua_tonumber(L, 9);
	glm::vec3 endPos = glm::vec3(end_x,end_y,end_z);
	float ctl_x = lua_tonumber(L, 10);
	float ctl_y = lua_tonumber(L, 11);
	float ctl_z = lua_tonumber(L, 12);
	glm::vec3 ctlPos = glm::vec3(ctl_x,ctl_y,ctl_z);
	float TimeToMove = lua_tonumber(L, 13);
	float easeIn = lua_tonumber(L, 14); 
	float easeOut = lua_tonumber(L, 15);

	iCommand* nComm = new cBezierCurve(name,type,&(pGO->positionXYZ),startPos,endPos,ctlPos,TimeToMove,easeIn,easeOut);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int Rotate(lua_State* L)
{	
	// cRotateTo_Time(std::string name, std::string type, cGameObject* pGO, 
	// 		glm::vec3 EulerAngles, float timeTR, int easeIn, int easeOut);
	const char* name = lua_tostring(L, 1);
	const char* type = lua_tostring(L, 2);
	const char* objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 eulerAngles = glm::vec3(start_x,start_y,start_z);
	float TimeToMove = lua_tonumber(L, 10);
	int easeIn = lua_tonumber(L, 11); 
	int easeOut = lua_tonumber(L, 12);

	iCommand* nComm = new cRotateTo_Time(name,type,pGO,eulerAngles,TimeToMove,easeIn,easeOut);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int Orient(lua_State* L)
{	
	// cOrientTo_Time(std::string name, std::string type, cGameObject* pGO,
	// glm::vec3 EulerAngles, float timeTR);
	const char* name = lua_tostring(L, 1);
	const char* type = lua_tostring(L, 2);
	const char* objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 eulerAngles = glm::vec3(start_x,start_y,start_z);
	float TimeToMove = lua_tonumber(L, 10);

	iCommand* nComm = new cOrientTo_Time(name,type,pGO,eulerAngles,TimeToMove);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int Trigger(lua_State* L)
{
	// cTrigger(std::string name, std::string type, 
	// 	glm::vec3* itemToMonitor, glm::vec3 startPos, iCommand* theCommand,
	// 	float radius, float TimeToExist = 10.0f);	
	const char* name = lua_tostring(L, 1);
	const char* type = lua_tostring(L, 2);
	const char* objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	glm::vec3* itemToMonitor;
	if(objt != "cam")
	{
		cGameObject* pGO = ::g_map_GameObjects[objt];
		itemToMonitor = &(pGO->positionXYZ);
	}
	else
	{
		itemToMonitor = &(::g_pFlyCamera->eye);
	}
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 startPos = glm::vec3(start_x,start_y,start_z);	
	const char* cmd = lua_tostring(L, 7);
	iCommand* iComm = cmdDictionary[cmd];
	float radius = lua_tonumber(L, 8);
	float TimeToMove = lua_tonumber(L, 9);

	iCommand* nComm = new cTrigger(name,type,itemToMonitor,startPos,iComm,radius,TimeToMove);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

