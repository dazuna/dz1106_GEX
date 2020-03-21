#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GFLW_callbacks.h"
#include "globalStuff.h"			// for finding object
#include "JSONLoader.h"
#include "util/tools.h"

#include "cLight.h"
#include <iostream>
#include <stdio.h>		// for fprintf()
#include <string>
#include <sstream>
#include "cFlyCamera/cFlyCamera.h"
#include "playerController/playerController.h"
#include "cLuaBrain/cLuaBrain.h"

#include "cAnimatedPlayer/cAnimatedPlayer.h"

bool isOnlyShiftKeyDown(int mods);
bool isOnlyCtrlKeyDown(int mods);
bool isOnlyAltKeyDown(int mods);
bool isShiftDown(GLFWwindow* window);
bool isCtrlDown(GLFWwindow* window);
bool isAltDown(GLFWwindow* window);
bool areAllModifiersUp(GLFWwindow* window);

void cameraControls(GLFWwindow* window);
void playerControls(GLFWwindow* window);

bool g_MouseIsInsideWindow = false;
bool g_MouseLeftButtonIsDown = false;

// Declared in theMain
extern cFlyCamera* g_pFlyCamera;
extern playerController* pPlayerControl;
extern bool cameraFollowPlayer;
extern cLuaBrain* p_LuaScripts;
extern std::map<std::string, iCommand*> cmdDictionary;
extern iCommand* masterCommandGroup;

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);
void getStatus();
void makeAllWireFrame(bool wireAll);


void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		::g_MouseIsInsideWindow = true;
		//std::cout << "Mouse moved inside window" << std::endl;
	}
	else
	{
		::g_MouseIsInsideWindow = false;
		//std::cout << "Mouse moved outside window" << std::endl;
	}
	return;
}

void stopPlayer(GLFWwindow* window, int key, int action, int mods)
{
	auto theAnimatedPlayer = cAnimatedPlayer::getAnimatedPlayer();
	
	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		{
			theAnimatedPlayer->nextPlayer();
		}
	}
	if (isShiftKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			theAnimatedPlayer->resetVelocity();
			theAnimatedPlayer->idle();
		}
	}
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::mat4 matRotX,matRotY;
	glm::vec3 invVisVec;
	glm::vec3 vecX1 = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 vecY1 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 vecZ1 = glm::vec3(0.0f, 0.0f, 1.0f);
	//cGameObject* playerSphere = ::g_map_GameObjects["spherePlayer"];
	const float CAMERASPEED = 1.0f;
	const float DEGREESOFROTATION = 3.0f;
	cGameObject* theSelectedGO = selectedGameObject->second;
	cLight* theSelectedL = (selectedLight->second);
	
	auto theAnimatedPlayer = cAnimatedPlayer::getAnimatedPlayer();
	bool debugControls = false;
	debugControls = !(theAnimatedPlayer->isPlayModeOn);

	if (debugControls)
	{
	
		if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
		{
			if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:
					selectedGameObject++;
					if (selectedGameObject == ::g_map_GameObjects.end()) 
						selectedGameObject = ::g_map_GameObjects.begin();
					break;
				case selectedType::LIGHT:
					selectedLight++;
					if (selectedLight == ::g_map_pLights.end()) 
						selectedLight = ::g_map_pLights.begin();
					break;
				case selectedType::SOUND:break;
				}
					
			}
			if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			{
				getStatus();
				std::cout << console << std::endl;
			}
			if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:cursorType = selectedType::LIGHT; break;
				case selectedType::LIGHT:cursorType = selectedType::GAMEOBJECT; break;
				case selectedType::SOUND:break;
				}
			}
			if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
			{
				isDroneOn = !isDroneOn;
			}
			if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
			{
				bool tempBool;
				tempBool = ::pSkyBox->isVisible;
				tempBool = !tempBool;
			}
			if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
			{
				::isBloom = !::isBloom;
				std::cout << "isBloom: " << isBloom << std::endl;
			}
			if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
			{
				::isNightVision = !::isNightVision;
				std::cout << "isNightVision: " << isNightVision << std::endl;
				
			}
			if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			{
				::g_map_GameObjects["cameraPosition0"]->positionXYZ = ::g_pFlyCamera->eye;
			}

			if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			{
				::g_map_GameObjects["cameraPosition0"]->positionXYZ = ::g_pFlyCamera->eye;
			}
		}

		if (isShiftKeyDownByAlone(mods))
		{
			// move the light
			if (key == GLFW_KEY_A)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.x += CAMERASPEED; break;
				case selectedType::LIGHT:		theSelectedL->positionXYZ.x += (CAMERASPEED*0.5f);	break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_D)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.x -= CAMERASPEED; break;
				case selectedType::LIGHT:		theSelectedL->positionXYZ.x -= (CAMERASPEED*0.5f);	break;
				case selectedType::SOUND:		break;
				}
			}

			// Move the camera (Q & E for up and down, along the y axis)
			if (key == GLFW_KEY_Q)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.y -= CAMERASPEED; break;
				case selectedType::LIGHT:		theSelectedL->positionXYZ.y -= (CAMERASPEED*0.5f);	break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_E)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.y += CAMERASPEED; break;
				case selectedType::LIGHT:		theSelectedL->positionXYZ.y += (CAMERASPEED*0.5f);	break;
				case selectedType::SOUND:		break;
				}
			}

			// Move the camera (W & S for towards and away, along the z axis)
			if (key == GLFW_KEY_W)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.z += CAMERASPEED; break;
				case selectedType::LIGHT:		theSelectedL->positionXYZ.z += (CAMERASPEED*0.5f);	break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_S)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.z -= CAMERASPEED; break;
				case selectedType::LIGHT:		theSelectedL->positionXYZ.z -= (CAMERASPEED*0.5f);	break;
				case selectedType::SOUND:		break;
				}
			}

			// ***************************** Scale Objects *****************************
			if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->scale -= 0.5f; break;
				case selectedType::LIGHT:		break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->scale += 0.5f; break;
				case selectedType::LIGHT:		break;
				case selectedType::SOUND:		break;
				}
			}
			// ***************************** Scale Objects *****************************

			//if (key == GLFW_KEY_1)
			//{
			//	theSelectedL->ConstAtten *= 0.99f;			// 99% of what it was
			//}
			//if (key == GLFW_KEY_2)
			//{
			//	theSelectedL->ConstAtten *= 1.01f;			// 1% more of what it was
			//}		
			if (key == GLFW_KEY_3)
			{
				theSelectedL->LinearAtten *= 0.90f;			// 99% of what it was
			}
			if (key == GLFW_KEY_4)
			{
				theSelectedL->LinearAtten *= 1.10f;			// 1% more of what it was
			}
			if (key == GLFW_KEY_5)
			{
				theSelectedL->QuadraticAtten *= 0.90f;			// 99% of what it was
			}
			if (key == GLFW_KEY_6)
			{
				theSelectedL->QuadraticAtten *= 1.10f;			// 1% more of what it was
			}
			//lightSwitch
			if (key == GLFW_KEY_0 && action == GLFW_PRESS)	
			{
				if (theSelectedL->lightSwitch == 1.0f)
				{
					theSelectedL->lightSwitch = 0.0f;
				}
				else 
				{
					theSelectedL->lightSwitch = 1.0f;
				}			
			}
			if (key == GLFW_KEY_V)
			{
				theSelectedL->innerAngle -= 0.1f;
			}
			if (key == GLFW_KEY_B)
			{
				theSelectedL->innerAngle += 0.1f;
			}
			if (key == GLFW_KEY_N)
			{
				theSelectedL->outerAngle -= 0.1f;
			}
			if (key == GLFW_KEY_M)
			{
				theSelectedL->outerAngle += 0.1f;
			}
			if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:
					if (selectedGameObject == ::g_map_GameObjects.begin())
						selectedGameObject = ::g_map_GameObjects.end();
					selectedGameObject--;
					break;
				case selectedType::LIGHT:
					if (selectedLight == ::g_map_pLights.begin())
						selectedLight = ::g_map_pLights.end();
					selectedLight--;
					break;
				case selectedType::SOUND:break;
				}
			}
			// <Object Rotation> ******************************************************
			if (key == GLFW_KEY_J && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->updateOrientation((vecZ1* -5.0f)); break;
				case selectedType::LIGHT:		theSelectedL->updateOrientation((vecZ1 * -5.0f));  break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_L && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->updateOrientation((vecZ1* 5.0f)); break;
				case selectedType::LIGHT:		theSelectedL->updateOrientation((vecZ1 * 5.0f)); break;
				case selectedType::SOUND:		break;
				}
			}

			if (key == GLFW_KEY_U && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->updateOrientation((vecY1* -5.0f)); break;
				case selectedType::LIGHT:		theSelectedL->updateOrientation((vecY1 * -5.0f)); break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_O && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->updateOrientation((vecY1* 5.0f)); break;
				case selectedType::LIGHT:		theSelectedL->updateOrientation((vecY1 * 5.0f)); break;
				case selectedType::SOUND:		break;
				}
			}

			if (key == GLFW_KEY_I && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->updateOrientation((vecX1* 5.0f)); break;
				case selectedType::LIGHT:		theSelectedL->updateOrientation((vecX1 * 5.0f)); break;
				case selectedType::SOUND:		break;
				}
			}
			if (key == GLFW_KEY_K && action == GLFW_PRESS)
			{
				switch (cursorType)
				{
				case selectedType::GAMEOBJECT:	theSelectedGO->updateOrientation((vecX1* -5.0f)); break;
				case selectedType::LIGHT:		theSelectedL->updateOrientation((vecX1 * -5.0f)); break;
				case selectedType::SOUND:		break;
				}
			}
			// </Object Rotation> ******************************************************
			
		}//if (isShiftKeyDownByAlone(mods))

		if (isCtrlKeyDownByAlone(mods))
		{
			if (key == GLFW_KEY_S && action == GLFW_PRESS)
			{
				JSONLoader::JSONSaveLights();
				JSONLoader::JSONSaveGos();
			}
			if (key == GLFW_KEY_W && action == GLFW_PRESS)
			{
				makeAllWireFrame(true);
			}
			if (key == GLFW_KEY_Q && action == GLFW_PRESS)
			{
				makeAllWireFrame(false);
			}
			if (key == GLFW_KEY_E && action == GLFW_PRESS)
			{
				::debugger = !::debugger;
			}
			if (key == GLFW_KEY_R && action == GLFW_PRESS)
			{
				::g_map_GameObjects.clear();
				JSONLoader::JSONLoadGameObjects(&::g_map_GameObjects);
				::selectedGameObject = ::g_map_GameObjects.begin();
				
				//::cmdDictionary.clear();
				//::masterCommandGroup = NULL;
				//::p_LuaScripts->LoadScript("./cLuaBrain/script.lua");
			}
			if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			{
				if (tools::pFindObjectByFriendlyNameMap("cameraPosition1"))
				{
					cameraEye = ::g_map_GameObjects["cameraPosition1"]->positionXYZ;
					if (tools::pFindObjectByFriendlyNameMap("cameraTarget1"))
					{
						cameraTarget = ::g_map_GameObjects["cameraTarget1"]->positionXYZ;
						visionVector = glm::normalize(cameraTarget - cameraEye);
					}
					cameraTarget = cameraEye + visionVector;
				}
			}
			if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			{
				if (tools::pFindObjectByFriendlyNameMap("cameraPosition2"))
				{
					cameraEye = ::g_map_GameObjects["cameraPosition2"]->positionXYZ;
					if (tools::pFindObjectByFriendlyNameMap("cameraTarget2"))
					{
						cameraTarget = ::g_map_GameObjects["cameraTarget2"]->positionXYZ;
						visionVector = glm::normalize(cameraTarget - cameraEye);
					}
					cameraTarget = cameraEye + visionVector;
				}
			}
			if (key == GLFW_KEY_3 && action == GLFW_PRESS)
			{
				if (tools::pFindObjectByFriendlyNameMap("cameraPosition3"))
				{
					cameraEye = ::g_map_GameObjects["cameraPosition3"]->positionXYZ;
					if (tools::pFindObjectByFriendlyNameMap("cameraTarget3"))
					{
						cameraTarget = ::g_map_GameObjects["cameraTarget3"]->positionXYZ;
						visionVector = glm::normalize(cameraTarget - cameraEye);
					}
					cameraTarget = cameraEye + visionVector;
				}
			}
		}
	}
	else
	{
		stopPlayer(window, key, action, mods);
	}
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if(key == GLFW_KEY_F4 && action == GLFW_PRESS)
	{
		theAnimatedPlayer->isPlayModeOn = !(theAnimatedPlayer->isPlayModeOn);
		std::cout << "play mode: " << theAnimatedPlayer->isPlayModeOn << std::endl;
	}
}

bool isShiftKeyDownByAlone(int mods)
{
	if (mods == GLFW_MOD_SHIFT)			
	{
		// Shift key is down all by itself
		return true;
	}
	return false;
}

bool isCtrlKeyDownByAlone(int mods)
{
	if (mods == GLFW_MOD_CONTROL)			
	{
		return true;
	}
	return false;
}

void getStatus()
{
	system("cls");
	std::ostringstream tempSS;
	std::map<float, cGameObject*>::iterator itGO2;
	switch (cursorType)
	{
	case selectedType::GAMEOBJECT:
		tempSS << "cursor: " << selectedGameObject->second->friendlyName
			<< " x: " << selectedGameObject->second->positionXYZ.x
			<< " y: " << selectedGameObject->second->positionXYZ.y
			<< " z: " << selectedGameObject->second->positionXYZ.z
			<< "\n";
		tools::printGameObject(selectedGameObject->second);
		for (itGO2 = ::closestTransparentObjects.begin(); itGO2 != ::closestTransparentObjects.end(); itGO2++)
		{
			tools::printGameObject(itGO2->second);
		}
		break;
	case selectedType::LIGHT:
		tempSS << "cursor: " << selectedLight->second->getName()
			<< " x: " << selectedLight->second->positionXYZ.x
			<< " y: " << selectedLight->second->positionXYZ.y
			<< " z: " << selectedLight->second->positionXYZ.z
			<< " type: " << selectedLight->second->type
			<< "\n";
		break;
	case selectedType::SOUND:break;
	}
	console = "";
	console += tempSS.str();
}

void makeAllWireFrame(bool wireAll)
{
	if (wireAll)
	{
		for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects.begin();
			itGO != ::g_map_GameObjects.end();
			itGO++)
		{
			itGO->second->isWireframe = true;
		}
	}
	else
	{
		for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects.begin();
			itGO != ::g_map_GameObjects.end();
			itGO++)
		{
			itGO->second->isWireframe = false;
		}
	}
}

bool isOnlyShiftKeyDown(int mods)
{
	if (mods == GLFW_MOD_SHIFT)
	{
		// Shift key is down all by itself
		return true;
	}
	//// Ignore other keys and see if the shift key is down
	//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
	//{

	//}
	return false;
}

bool isOnlyCtrlKeyDown(int mods)
{
	if (mods == GLFW_MOD_CONTROL)
	{
		return true;
	}
	return false;
}

bool isOnlyAltKeyDown(int mods)
{
	if (mods == GLFW_MOD_ALT)
	{
		return true;
	}
	return false;
}

bool isShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

// Mouse (cursor) callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{return;}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	::g_pFlyCamera->setMouseWheelDelta(yoffset);
	//	std::cout << "Mouse wheel: " << yoffset << std::endl;
	return;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		::g_MouseLeftButtonIsDown = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		::g_MouseLeftButtonIsDown = false;
	}
	return;
}

void ProcessAsyncMouse(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	::g_pFlyCamera->setMouseXY(x, y);

	const float MOUSE_SENSITIVITY = 0.1f;

	//	std::cout << ::g_pFlyCamera->getMouseX() << ", " << ::g_pFlyCamera->getMouseY() << std::endl;

		// Mouse left (primary?) button pressed? 
		// AND the mouse is inside the window...
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		&& ::g_MouseIsInsideWindow)
	{
		// Mouse button is down so turn the camera
		::g_pFlyCamera->Yaw_LeftRight(-::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY);

		::g_pFlyCamera->Pitch_UpDown(::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY);

	}

	// Adjust the mouse speed
	if (::g_MouseIsInsideWindow)
	{
		const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

		// Adjust the movement speed based on the wheel position
		::g_pFlyCamera->movementSpeed += (::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY);
		if (::g_pFlyCamera->movementSpeed <= 0.0f)
		{
			::g_pFlyCamera->movementSpeed = 0.0f;
		}
	}


	// HACK 
	::g_pFlyCamera->movementSpeed = 2.0f;

	return;
}//ProcessAsyncMouse(...

void ProcessAsyncKeys(GLFWwindow* window)
{
	// WASD + q = "up", e = down		y axis = up and down
	//									x axis = left and right
	//									z axis = forward and backward
	//float cameraSpeed = CAMERA_MOVE_SPEED_SLOW;
	//if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS  )
	//{
	//	cameraSpeed = CAMERA_MOVE_SPEED_FAST;
	//}

	auto theAnimatedPlayer = cAnimatedPlayer::getAnimatedPlayer();
	bool debugControls = !(theAnimatedPlayer->isPlayModeOn);

	if (debugControls)
	{
		cameraControls(window);
	}
	else
	{
		playerControls(window);		
	}
	
}// ProcessAsyncKeys(..

void cameraControls(GLFWwindow* window)
{
	const float CAMERA_MOVE_SPEED_SLOW = 0.1f;
	const float CAMERA_MOVE_SPEED_FAST = 1.0f;
	const float CAMERA_TURN_SPEED = 0.1f;
	
	float cameraMoveSpeed = ::g_pFlyCamera->movementSpeed;
	float playerVSpeed = 2.0f;
	float playerAngle = 0.5f;
	
	// If no keys are down, move the camera
	if (areAllModifiersUp(window))
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			//			g_CameraEye.z += cameraSpeed;
			::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			//			g_CameraEye.z -= cameraSpeed;
			::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			//			g_CameraEye.x -= cameraSpeed;
			::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			//			g_CameraEye.x += cameraSpeed;
			::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
			//			::g_pFlyCamera->Roll_CW_CCW( +cameraSpeed );
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			//			g_CameraEye.y -= cameraSpeed;
			::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
			//			::g_pFlyCamera->Roll_CW_CCW( -cameraSpeed );
		}

	}//if(AreAllModifiersUp(window)
	
	if (isShiftDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			::g_pFlyCamera->Roll_CW_CCW(-CAMERA_TURN_SPEED);
			//			::g_pFlyCamera->MoveUpDown_Y( +cameraSpeed );
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			::g_pFlyCamera->Roll_CW_CCW(+CAMERA_TURN_SPEED);
			//			::g_pFlyCamera->MoveUpDown_Y( -cameraSpeed );
		}
	}//IsShiftDown(window)
}

void playerControls(GLFWwindow* window)
{
	auto theAnimatedPlayer = cAnimatedPlayer::getAnimatedPlayer();
	
	if (areAllModifiersUp(window))
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			theAnimatedPlayer->moveAhead();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			theAnimatedPlayer->moveBack();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			theAnimatedPlayer->leftStrafe();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			theAnimatedPlayer->rightStrafe();
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			auto theGO = *(theAnimatedPlayer->selectedPlayable);
			theGO->updateOrientation(glm::vec3(0,0.5f,0));
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			auto theGO = *(theAnimatedPlayer->selectedPlayable);
			theGO->updateOrientation(glm::vec3(0,-0.5f,0));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			theAnimatedPlayer->jump();
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			theAnimatedPlayer->specialMove();
		}
		

	}//if(AreAllModifiersUp(window)
	
	if (isShiftDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			theAnimatedPlayer->runAhead();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			theAnimatedPlayer->runBack();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			theAnimatedPlayer->rollLeft();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			theAnimatedPlayer->rollRight();
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			auto theGO = *(theAnimatedPlayer->selectedPlayable);
			theGO->updateOrientation(glm::vec3(0,1.5f,0));
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			auto theGO = *(theAnimatedPlayer->selectedPlayable);
			theGO->updateOrientation(glm::vec3(0,-1.5f,0));
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			theAnimatedPlayer->jump();
		}
	}//IsShiftDown(window)
}