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

-- MoveTo("mvSph1","move","sphere",    0,0,0,    -30,0,0,    10,2,2)
-- MoveLightTo("mvlgt1","move","spotlight1",    0,0,0,    -30,0,0,    10,2,2)
-- Follow("flSph2","foll","sphere2","sphere",12,    0,10,-8,    10,3,10)
-- FollowLight("flLgt2","foll","spotlight1","sphere",12,    0,10,0,    10,3,10)
-- Curve("crvSph2","curv","sphere2",    8,10,0,    -8,10,0,    0,0,0,    8,2,2)
-- Rotate("rtTie","rott","tie1",    0,90,0,    5,0,0)
-- RotateLight("rtLgt1","rott","spotlight1",    90,0,90,    5,0,0)
-- Orient("orTie","ornt","tie1",    0,-1,0,    3)
-- Trigger()

-- MoveTo("mvSph2","move","sphere",     25.0, 18.0, 0.0,     -25.0, 18.0, 0.0,    10,2,2)
-- MoveTo("mvSph1","move","sphere2",    -25.0, 16.0, 0.0,    25.0, 16.0, 0.0,    10,2,2)
-- MoveTo("mvSph3","move","sphere3",    25.0, 14.0, 0.0,     -25.0, 14.0, 0.0,    10,2,2)
-- -- Follow("mvSph4","foll","sphere4","sphere",15,     0,1,0,   10,1,10)
-- -- Follow("mvSph5","foll","sphere5","sphere2",15,    0,1,0,   10,1,10)
-- -- Follow("mvSph6","foll","sphere6","sphere3",15,    0,1,0,   10,1,10)
-- MoveTo("mvSph4","move","sphere4",     -25.0, 12.0, 0.0,    25.0, 12.0, 0.0,    10,3,3)
-- MoveTo("mvSph5","move","sphere5",    25.0, 10.0, 0.0,     -25.0, 10.0, 0.0,    10,3,3)
-- MoveTo("mvSph6","move","sphere6",     -25.0, 8.0, 0.0,     25.0, 8.0, 0.0,    10,3,3)

-- addToParallel("crvTie1","follSphere")
-- addToParallel("rtTie2","follSphere")
-- addToSerial("follSphere","shipNavigation")
-- addToSerial("mvShp2","shipNavigation")

MoveTo("mvCam","move","cam",    340,271,-325,    250,150,190,   5,1,1)
CamLookAt("camLook","lookC","pirateShip2",7)
newParallel("shipNavigation","mainCG")
addToParallel("camLook","shipNavigation")
addToParallel("mvCam","shipNavigation")
setTheCommandMasterToRuleThemAll("shipNavigation")
