#include "cLuaToCommand.hpp"

int KillAllHumans(lua_State* L)
{
	// There are 3 things on the stack: "Justin", 47, 3.14159 
	std::string name = lua_tostring(L, 1);	// get "Justin"		
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
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
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

	cGameObject* pGO;
	glm::vec3* pFollowerVec3;
	if(objt != "cam")
	{
		pGO = ::g_map_GameObjects[objt];
		pFollowerVec3 = &(pGO->positionXYZ);
	}
	else
	{
		pFollowerVec3 = &(::g_pFlyCamera->eye);
	}

	iCommand* nComm = new cMoveTo_AB_Time(name,type,pFollowerVec3,startPos,endPos,TimeToMove,easeIn,easeOut);
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
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);	
	std::string objt2 = lua_tostring(L, 4);
	cGameObject* pGO2 = ::g_map_GameObjects[objt2];
	float TimeToFollow = lua_tonumber(L, 5);
	float offset_x = lua_tonumber(L, 6);
	float offset_y = lua_tonumber(L, 7);
	float offset_z = lua_tonumber(L, 8);
	glm::vec3 offsetVec = glm::vec3(offset_x,offset_y,offset_z);
	float maxDist = lua_tonumber(L, 9);
	float minDist = lua_tonumber(L, 10); 
	float maxSpeed = lua_tonumber(L, 11);

	cGameObject* pGO;
	glm::vec3* pFollowerVec3;
	glm::vec3* targetItem = &(pGO2->positionXYZ);
	if(objt != "cam")
	{
		pGO = ::g_map_GameObjects[objt];
		pFollowerVec3 = &(pGO->positionXYZ);
	}
	else
	{
		pFollowerVec3 = &(::g_pFlyCamera->eye);
	}
	iCommand* nComm = new cFollowObject(name,type,pFollowerVec3,targetItem,
		TimeToFollow,offsetVec,maxDist,minDist,maxSpeed);
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
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
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
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 eulerAngles = glm::vec3(start_x,start_y,start_z);
	float TimeToMove = lua_tonumber(L, 7);
	int easeIn = lua_tonumber(L, 8); 
	int easeOut = lua_tonumber(L, 9);

	iCommand* nComm = new cRotateTo_Time(name,type,pGO,eulerAngles,TimeToMove,easeIn,easeOut);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int Orient(lua_State* L)
{	
	// cOrientTo_Time(std::string name, std::string type, cGameObject* pGO,
	// glm::vec3 EulerAngles, float timeTR);
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 eulerAngles = glm::vec3(start_x,start_y,start_z);
	float TimeToMove = lua_tonumber(L, 7);

	iCommand* nComm = new cOrientTo_Time(name,type,pGO,eulerAngles,TimeToMove);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int Trigger(lua_State* L)
{
	// cTrigger(std::string name, std::string type, 
	// 	glm::vec3* itemToMonitor, glm::vec3 startPos, iCommand* theCommand,
	// 	float radius, float TimeToExist = 10.0f);	
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
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
	std::string cmd = lua_tostring(L, 7);
	iCommand* iComm = cmdDictionary[cmd];
	float radius = lua_tonumber(L, 8);
	float TimeToMove = lua_tonumber(L, 9);

	iCommand* nComm = new cTrigger(name,type,itemToMonitor,startPos,iComm,radius,TimeToMove);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int CamLookAt(lua_State* L)
{
	// cCamLookAt(std::string name, std::string type, 
	// glm::vec3* posXYZ, float TimeToLook);
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);

	std::string objt = lua_tostring(L, 3);
	cGameObject* pGO = ::g_map_GameObjects[objt];
	glm::vec3* itemToMonitor;
	itemToMonitor = &(pGO->positionXYZ);

	float TimeToMove = lua_tonumber(L, 4);

	iCommand* nComm = new cCamLookAt(name,type,itemToMonitor,TimeToMove);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

// light specifics
int MoveLightTo(lua_State* L)
{
	// cMoveTo_AB_Time
	// (std::string name, std::string type, 
	// cGameObject* pGO, glm::vec3 startPosition, 
	// glm::vec3 endPosition, float TimeToMove, 
	// float easeIn, float easeOut);
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 startPos = glm::vec3(start_x, start_y, start_z);
	float end_x = lua_tonumber(L, 7);
	float end_y = lua_tonumber(L, 8);
	float end_z = lua_tonumber(L, 9);
	glm::vec3 endPos = glm::vec3(end_x, end_y, end_z);
	float TimeToMove = lua_tonumber(L, 10);
	float easeIn = lua_tonumber(L, 11);
	float easeOut = lua_tonumber(L, 12);

	cLight* pGO;
	glm::vec3* pFollowerVec3;
	pGO = &(::g_map_pLights[objt]);
	pFollowerVec3 = &(pGO->positionXYZ);

	iCommand* nComm = new cMoveTo_AB_Time(name, type, pFollowerVec3, startPos, endPos, TimeToMove, easeIn, easeOut);
	::cmdDictionary.insert({ name,nComm });

	return 0;
}

int FollowLight(lua_State* L)
{
	// cFollowObject(std::string name, std::string type,
	// 	glm::vec3* followerPos, glm::vec3* targetPos,
	// 	float TimeToFollow, glm::vec3 offsetTarget,
	// 	float maxDistance_MaxVelocity, float minDistance_ZeroVelocity,
	// 	float maxSpeed);
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);	
	std::string objt2 = lua_tostring(L, 4);
	cGameObject* pGO2 = ::g_map_GameObjects[objt2];
	float TimeToFollow = lua_tonumber(L, 5);
	float offset_x = lua_tonumber(L, 6);
	float offset_y = lua_tonumber(L, 7);
	float offset_z = lua_tonumber(L, 8);
	glm::vec3 offsetVec = glm::vec3(offset_x,offset_y,offset_z);
	float maxDist = lua_tonumber(L, 9);
	float minDist = lua_tonumber(L, 10); 
	float maxSpeed = lua_tonumber(L, 11);

	cLight* pGO;
	glm::vec3* pFollowerVec3;
	pGO = &(::g_map_pLights[objt]);
	pFollowerVec3 = &(pGO->positionXYZ);
	glm::vec3* targetItem = &(pGO2->positionXYZ);

	iCommand* nComm = new cFollowObject(name,type,pFollowerVec3,targetItem,
		TimeToFollow,offsetVec,maxDist,minDist,maxSpeed);
	::cmdDictionary.insert({name,nComm});

	return 0;
}

int RotateLight(lua_State* L)
{
	// cRotateTo_Time(std::string name, std::string type, cGameObject* pGO, 
	// 		glm::vec3 EulerAngles, float timeTR, int easeIn, int easeOut);
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	std::string objt = lua_tostring(L, 3);
	cLight* pGO = &(::g_map_pLights[objt]);
	float start_x = lua_tonumber(L, 4);
	float start_y = lua_tonumber(L, 5);
	float start_z = lua_tonumber(L, 6);
	glm::vec3 eulerAngles = glm::vec3(start_x, start_y, start_z);
	float TimeToMove = lua_tonumber(L, 7);
	int easeIn = lua_tonumber(L, 8);
	int easeOut = lua_tonumber(L, 9);

	iCommand* nComm = new cRotateTo_Light(name, type, pGO, eulerAngles, TimeToMove, easeIn, easeOut);
	::cmdDictionary.insert({ name,nComm });

	return 0;
}

int newSerial(lua_State* L)
{
	//newSerial(name,type)
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	cCommandGroupSerial* CGSerial = new cCommandGroupSerial(name, type);
	::cmdDictionary.insert({name,CGSerial});
}

int newParallel(lua_State* L)
{
	//int newParallel(name,type)
	std::string name = lua_tostring(L, 1);
	std::string type = lua_tostring(L, 2);
	cCommandGroupParallel* CGParallel = new cCommandGroupParallel(name, type);
	::cmdDictionary.insert({name,CGParallel});
}

//CGSerial->AddCommandSerial(moveTo);
int addToSerial(lua_State* L)
{
	// addToSerial(name,cmdGrp)
	std::string name = lua_tostring(L, 1);
	std::string cmdGrp = lua_tostring(L, 2);
	cCommandGroupSerial* CGSerial = (cCommandGroupSerial*)::cmdDictionary[cmdGrp];
	CGSerial->AddCommandSerial(::cmdDictionary[name]);
}

//CGParallel->AddCommandParallel(moveTo);
int addToParallel(lua_State* L)
{
	// addToParallel(name,cmdGrp)
	std::string name = lua_tostring(L, 1);
	std::string cmdGrp = lua_tostring(L, 2);
	cCommandGroupParallel* CGParallel = (cCommandGroupParallel*)::cmdDictionary[cmdGrp];
	CGParallel->AddCommandParallel(::cmdDictionary[name]);
}

int setTheCommandMasterToRuleThemAll(lua_State* L)
{
	// sTCMTRTA(cmdGrp)
	std::string cmdGrp = lua_tostring(L, 1);
	::masterCommandGroup = ::cmdDictionary[cmdGrp];
}
