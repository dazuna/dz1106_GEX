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


-- Start of First Volley    -----------------------------------

MoveTo("mvSphp1","move","pirateShip1",    -30,182,-77,    -30,183,-77,   0.5,0,0)
Rotate("rtShp1","rott","pirateShip1",    0,-45,0,    0.1,0,0)
MoveTo("mvSph0","move","sphere0",    -30,182,-79,    -108,164,-79,   3,0,0)
MoveTo("mvSph1","move","sphere1",    -30,182,-78,    -108,164,-78,   3,0,0)
MoveTo("mvSph2","move","sphere2",    -30,182,-77,    -108,164,-77,   3,0,0)
MoveTo("mvSph3","move","sphere3",    -30,182,-76,    -108,164,-76,   3,0,0)

newSerial("firstVolley","mainCG")
addToSerial("rtShp1","firstVolley")
addToSerial("mvSphp1","firstVolley")
addToSerial("mvSph0","firstVolley")
addToSerial("mvSph1","firstVolley")
addToSerial("mvSph2","firstVolley")
addToSerial("mvSph3","firstVolley")

-- End of First Volley  -----------------------------------

MoveTo("mkTime","move","cube",    0,0,0,    -121.0, 181.0, -34.0,   10,1,0)

-- Start of Second Volley    -----------------------------------

MoveTo("mvSph4","move","sphere0",    -30,182,-79,    -108,175,-79,   3,0,0)
MoveTo("mvSph5","move","sphere1",    -30,182,-78,    -108,178,-78,   3,0,0)
MoveTo("mvSph6","move","sphere2",    -30,182,-77,    -108,181,-77,   3,0,0)
MoveTo("mvSph7","move","sphere3",    -30,182,-76,    -108,184,-76,   3,0,0)

MoveTo("mvSph8","move","sphere0",    0,0,0,    0,1,0,   0.1,0,0)
MoveTo("mvSph9","move","sphere1",    0,0,0,    0,1,0,   0.1,0,0)
MoveTo("mvSph10","move","sphere2",   0,0,0,    0,1,0,   0.1,0,0)
MoveTo("mvSph11","move","sphere3",   0,0,0,    0,1,0,   0.1,0,0)

newSerial("secondVolley","mainCG")
addToSerial("mkTime","secondVolley")
addToSerial("mvSph4","secondVolley")
addToSerial("mvSph8","secondVolley")
addToSerial("mvSph5","secondVolley")
addToSerial("mvSph9","secondVolley")
addToSerial("mvSph6","secondVolley")
addToSerial("mvSph10","secondVolley")
addToSerial("mvSph7","secondVolley")
addToSerial("mvSph11","secondVolley")

-- End of Second Volley    -----------------------------------

newSerial("daleVolleydale","mainCG")
addToSerial("firstVolley","daleVolleydale")
addToSerial("secondVolley","daleVolleydale")

-- End of Both Volleys    -----------------------------------

MoveTo("mkTime2","move","cube",    0,0,0,    -121.0, 181.0, -34.0,   15,1,0)
MoveTo("mvSphp2","move","pirateShip2",    -108,164,-75,    -108,185,-75,   25,0,0)

newSerial("ascendDefender","mainCG")
addToSerial("mkTime2","ascendDefender")
addToSerial("mvSphp2","ascendDefender")

newParallel("scene5","master")
addToParallel("ascendDefender","scene5")
addToParallel("daleVolleydale","scene5")

-- Putting all together :D ---------------------------------

setTheCommandMasterToRuleThemAll("scene5")
