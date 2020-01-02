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

-- arrange scene
MoveTo("mvShp2_1","move","pirateShip1",    19,182,186,    19,183,186,   0.1,0,0)
MoveTo("mvShp1_1","move","pirateShip2",    90,185,198,    90,186,198,   0.1,0,0)
Rotate("rtShp1","rott","pirateShip1",    0,-130,0,    0.1,0,0)
Rotate("rtShp2","rott","pirateShip2",    0,80,0,    0.1,0,0)

newParallel("arrangeScene","bureaucracy")
addToParallel("mvShp2_1","arrangeScene")
addToParallel("mvShp1_1","arrangeScene")
addToParallel("rtShp1","arrangeScene")
addToParallel("rtShp2","arrangeScene")

-- end arrange 

MoveTo("shot","move","sphere0",    90,185,198,    19,182,186,   3,0,0)
MoveTo("shotdisappear","move","sphere0",    0,0,0,    1,0,0,   0.2,0,0)

-- end Shot!
MoveTo("mkTime","move","sphere1",    0,0,0,    0,1,0,   5,0,0)
MoveTo("mvCam","move","cam",    -10,190,186,    120,10,197,   10,0,1)
CamLookAt("camLook","lookC","pirateShip1",10)

newParallel("firstPartCam","fPC") -- first part cam
addToParallel("mvCam","firstPartCam")
addToParallel("camLook","firstPartCam")

MoveTo("putMarker","move","cube",    152,-20,197,    152,-19,197,   0.1,0,0)

Follow("follDefender","foll",    "cube","pirateShip2",20,    0,1,0,   10,1,30)
CamLookAt("camLook2","lookC","cube",10)

newParallel("secondPartCam","fPC") -- first part cam
addToParallel("follDefender","secondPartCam")
addToParallel("camLook2","secondPartCam")

newSerial("wholeCamThingy","WCT")
addToSerial("mkTime","wholeCamThingy")
addToSerial("firstPartCam","wholeCamThingy")
addToSerial("putMarker","wholeCamThingy")
addToSerial("secondPartCam","wholeCamThingy")

-- end Camera Stuff

MoveTo("mvShp2_2","move","pirateShip1",    19,182,186,    152,-20,197,   10,0,1)
MoveTo("mvShp1_2","move","pirateShip2",    90,185,198,    400,185,198,   20,0,1)

newParallel("ship3Navigation","mainCG")
addToParallel("mvShp2_2","ship3Navigation")
addToParallel("mvShp1_2","ship3Navigation")

newSerial("FirstMovesShips","FMS")
addToSerial("arrangeScene","FirstMovesShips")
addToSerial("shot","FirstMovesShips")
addToSerial("shotdisappear","FirstMovesShips")
addToSerial("ship3Navigation","FirstMovesShips")

-- put everything together
newParallel("Everything","master")
addToParallel("wholeCamThingy","Everything")
addToParallel("FirstMovesShips","Everything")


setTheCommandMasterToRuleThemAll("Everything")
