-- Guide to commands:
-- MoveTo() -> cMoveTo_AB_Time(std::string name, std::string type, 
--      cGameObject* pGO, glm::vec3 startPosition, 
--      glm::vec3 endPosition, float TimeToMove, 
--      float easeIn, float easeOut);
-- Follow() -> cFollowObject(std::string name, std::string type,
-- 	    glm::vec3* followerPos, glm::vec3* targetPos,
-- 	    float TimeToFollow, glm::vec3 offsetTarget,
-- 	    float maxDistance_MaxVelocity, float minDistance_ZeroVelocity,
-- 	    float maxSpeed);
-- Curve()  -> cBezierCurve(std::string name, std::string type,
-- 	    glm::vec3* itemToMove,
-- 	    glm::vec3 startPos, glm::vec3 endPos,
-- 	    glm::vec3 ctlPos, float timeToMove,
-- 	    float easeIn, float easeOut);
-- Rotate() -> cRotateTo_Time(std::string name, std::string type, cGameObject* pGO, 
-- 		glm::vec3 EulerAngles, float timeTR, int easeIn, int easeOut);
-- Orient() -> cOrientTo_Time(std::string name, std::string type, cGameObject* pGO,
--      glm::vec3 EulerAngles, float timeTR);
-- Trigger()    -> cTrigger(std::string name, std::string type, 
-- 	    glm::vec3* itemToMonitor, glm::vec3 startPos, iCommand* theCommand,
-- 	    float radius, float TimeToExist = 10.0f);
-- cCamLookAt(std::string name, std::string type, glm::vec3* posXYZ, float TimeToLook)
-- newSerial()  -> newSerial(name,type)
-- newParallel()    -> newParallel(name,type)
-- addToSerial()    -> addToSerial(name,cmdGrp)
-- addToParallel()  -> addToParallel(name,cmdGrp)
-- setTheCommandMasterToRuleThemAll()  -> sTCMTRTA(cmdGrp)



MoveTo("mvShp2","move","pirateShip2",    -108,185,-75,    -108,185,200,   20,1,0)
Rotate("rtShp2","rott","pirateShip2",    0,80,0,    20,0,0)

Curve("crvShp1","curv","pirateShip1",    -30,182,-77,    -107,182,-154,    -53,182,-134,    10,1,0)
Curve("crvShp2","curv","pirateShip1",    -107,182,-154,    -189,182,-24,    -178,182,-95,    10,0,0)
Rotate("rtShp1","rott","pirateShip1",    0,170,0,    20,0,0)

newSerial("completeCurveShip1","ccCurve")
addToSerial("crvShp1","completeCurveShip1")
addToSerial("crvShp2","completeCurveShip1")

newParallel("ship2Navigation","mainCG")
addToParallel("mvShp2","ship2Navigation")
addToParallel("rtShp2","ship2Navigation")
addToParallel("rtShp1","ship2Navigation")
addToParallel("completeCurveShip1","ship2Navigation")

-- End of first Stage

MoveTo("mvShp2_2","move","pirateShip2",    -108,185,200,    270,185,198,   20,0,1)
MoveTo("mvShp1_2","move","pirateShip1",    -189,182,-24,    133,182,186,   20,0,1)

newParallel("ship3Navigation","mainCG")
addToParallel("mvShp2_2","ship3Navigation")
addToParallel("mvShp1_2","ship3Navigation")

-- End of Second Stage

newSerial("bothMovements","BMOVs")
addToSerial("ship2Navigation","bothMovements")
addToSerial("ship3Navigation","bothMovements")

-- Binding Both Movements in a Serial

CamLookAt("camLook","lookC","pirateShip1",42)
MoveTo("mvCam","move","cam",    -100,202,96,    -250,202,96,   20,1,1)

-- Camera Magic

newParallel("MasterCMD","master")
addToParallel("bothMovements","MasterCMD")
addToParallel("camLook","MasterCMD")
addToParallel("mvCam","MasterCMD")

setTheCommandMasterToRuleThemAll("MasterCMD")
