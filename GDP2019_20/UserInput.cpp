#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GFLW_callbacks.h"
#include "globalStuff.h"			// for finding object
#include "JSONLoader.h"

#include "cLight.h"
#include <iostream>
#include <stdio.h>		// for fprintf()
#include <string>
#include <sstream>

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);
void getStatus();
void makeAllWireFrame(bool wireAll);


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
	cLight* theSelectedL = &(selectedLight->second);

	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{
		// <Camera Movement> ******************************************************
		if (key == GLFW_KEY_A)
		{
			cameraEye.x -= CAMERASPEED;
			cameraTarget = cameraEye + visionVector;
		}
		if (key == GLFW_KEY_D)
		{
			cameraEye.x += CAMERASPEED;
			cameraTarget = cameraEye + visionVector;
		}

		if (key == GLFW_KEY_Q)
		{
			cameraEye.y -= CAMERASPEED;
			cameraTarget = cameraEye + visionVector;
		}
		if (key == GLFW_KEY_E)
		{
			cameraEye.y += CAMERASPEED;
			cameraTarget = cameraEye + visionVector;
		}

		if (key == GLFW_KEY_W)
		{
			//cameraEye.z -= CAMERASPEED;
			cameraEye = cameraEye + glm::normalize(visionVector);
			cameraTarget = cameraEye + visionVector;
		}
		if (key == GLFW_KEY_S)
		{
			//cameraEye.z += CAMERASPEED;
			cameraEye = cameraEye - glm::normalize(visionVector);
			cameraTarget = cameraEye + visionVector;
		}
		// </Camera Movement> ******************************************************

		// <Camera Rotattion> ******************************************************
		if (key == GLFW_KEY_DOWN)
		{
			matRotX = glm::rotate(glm::mat4(1.0f), -glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			visionVector = glm::vec3(matRotX * glm::vec4(visionVector, 1.0));
			cameraTarget = cameraEye + visionVector;
		}
		if (key == GLFW_KEY_UP)
		{
			matRotX = glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			visionVector = glm::vec3(matRotX * glm::vec4(visionVector, 1.0));
			cameraTarget = cameraEye + visionVector;
		}
		if (key == GLFW_KEY_LEFT)
		{
			matRotY = glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			visionVector = glm::vec3(matRotY * glm::vec4(visionVector, 1.0));
			cameraTarget = cameraEye + visionVector;
		}
		if (key == GLFW_KEY_RIGHT)
		{
			matRotY = glm::rotate(glm::mat4(1.0f), -glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			visionVector = glm::vec3(matRotY * glm::vec4(visionVector, 1.0));
			cameraTarget = cameraEye + visionVector;
		}
		// </Camera Rotattion> ******************************************************

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
		if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		{
			::g_map_GameObjects["sphere"]->positionXYZ = glm::vec3(2.0f, 10.0f, 0.0f);
			::g_map_GameObjects["sphere"]->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	if (isShiftKeyDownByAlone(mods))
	{
		// move the light
		if (key == GLFW_KEY_A)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.x -= CAMERASPEED; break;
			case selectedType::LIGHT:		theSelectedL->positionXYZ.x -= (CAMERASPEED*0.5f);	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_D)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.x += CAMERASPEED; break;
			case selectedType::LIGHT:		theSelectedL->positionXYZ.x += (CAMERASPEED*0.5f);	break;
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
			case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.z -= CAMERASPEED; break;
			case selectedType::LIGHT:		theSelectedL->positionXYZ.z -= (CAMERASPEED*0.5f);	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_S)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->positionXYZ.z += CAMERASPEED; break;
			case selectedType::LIGHT:		theSelectedL->positionXYZ.z += (CAMERASPEED*0.5f);	break;
			case selectedType::SOUND:		break;
			}
		}
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
			theSelectedL->LinearAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_4)
		{
			theSelectedL->LinearAtten *= 1.01f;			// 1% more of what it was
		}
		if (key == GLFW_KEY_5)
		{
			theSelectedL->QuadraticAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_6)
		{
			theSelectedL->QuadraticAtten *= 1.01f;			// 1% more of what it was
		}
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
		//if (key == GLFW_KEY_9)
		//{
		//	bLightDebugSheresOn = false;			
		//}
		//if (key == GLFW_KEY_0)
		//{
		//	bLightDebugSheresOn = true; 
		//}
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
			case selectedType::GAMEOBJECT:	theSelectedGO->rotationXYZ += (vecZ1*0.1f); break;
			case selectedType::LIGHT:		theSelectedL->direction += (vecZ1*0.1f);	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->rotationXYZ -= (vecZ1*0.1f); break;
			case selectedType::LIGHT:		theSelectedL->direction -= (vecZ1*0.1f);	break;
			case selectedType::SOUND:		break;
			}
		}

		if (key == GLFW_KEY_U && action == GLFW_PRESS)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->rotationXYZ += (vecY1*0.1f); break;
			case selectedType::LIGHT:		theSelectedL->direction += (vecY1*0.1f);	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->rotationXYZ -= (vecY1*0.1f); break;
			case selectedType::LIGHT:		theSelectedL->direction -= (vecY1*0.1f);	break;
			case selectedType::SOUND:		break;
			}
		}

		if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->rotationXYZ -= (vecX1*0.1f); break;
			case selectedType::LIGHT:		theSelectedL->direction -= (vecX1*0.1f);	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	theSelectedGO->rotationXYZ += (vecX1*0.1f); break;
			case selectedType::LIGHT:		theSelectedL->direction += (vecX1*0.1f);	break;
			case selectedType::SOUND:		break;
			}
		}
		// </Object Rotation> ******************************************************

		// <Color Moving> **********************************************************
		// move the light
		if (key == GLFW_KEY_UP)
		{
			theSelectedGO->objectColourRGBA.r -= 0.2f;
		}
		if (key == GLFW_KEY_UP)
		{
			theSelectedGO->objectColourRGBA.r += 0.2f;
		}

		// Move the camera (Q & E for up and down, along the y axis)
		if (key == GLFW_KEY_LEFT)
		{
			theSelectedGO->objectColourRGBA.g += 0.2f;
		}
		if (key == GLFW_KEY_RIGHT)
		{
			theSelectedGO->objectColourRGBA.g -= 0.2f;
		}

		// Move the camera (W & S for towards and away, along the z axis)
		if (key == GLFW_KEY_PERIOD)
		{
			theSelectedGO->objectColourRGBA.b += 0.2f;
		}
		if (key == GLFW_KEY_COMMA)
		{
			theSelectedGO->objectColourRGBA.b -= 0.2f;
		}
		// </Color Moving>****************************************************************************

	}//if (isShiftKeyDownByAlone(mods))

	if (isCtrlKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			JSONSaveLights(&::g_map_pLights);
			JSONSaveGameObjects(&::g_map_GameObjects);
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			makeAllWireFrame(true);
		}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			makeAllWireFrame(false);
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			::g_map_pLights["light8"].lightSwitch = 1.0f;
			::g_map_pLights["light9"].lightSwitch = 1.0f;
		}
		if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			::g_map_pLights["light8"].lightSwitch = 0.0f;
			::g_map_pLights["light9"].lightSwitch = 0.0f;
		}
		//if (key == GLFW_KEY_D && action == GLFW_PRESS)
		//{
		//	cGameObject* tempGO = new cGameObject(selectedGameObject->second);
		//	cLight* tempLight = new cLight(selectedLight->second);
		//	switch (cursorType)
		//	{
		//	case selectedType::GAMEOBJECT:				
		//		::g_map_GameObjects.insert({ tempGO->friendlyName.c_str(), tempGO });
		//		break;
		//	case selectedType::LIGHT:		
		//		::g_map_pLights.insert({ tempLight->getName(),*tempLight });
		//		break;
		//	case selectedType::SOUND:		break;
		//	}
		//}
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

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
	switch (cursorType)
	{
	case selectedType::GAMEOBJECT:
		tempSS << "cursor: " << selectedGameObject->second->friendlyName
			<< " x: " << selectedGameObject->second->positionXYZ.x
			<< " y: " << selectedGameObject->second->positionXYZ.y
			<< " z: " << selectedGameObject->second->positionXYZ.z
			<< "\n";
		break;
	case selectedType::LIGHT:
		tempSS << "cursor: " << selectedLight->second.getName()
			<< " x: " << selectedLight->second.positionXYZ.x
			<< " y: " << selectedLight->second.positionXYZ.y
			<< " z: " << selectedLight->second.positionXYZ.z
			<< " type: " << selectedLight->second.type
			<< "\n";
		break;
	case selectedType::SOUND:break;
	}
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

// <Sphere Movement> ******************************************************
		//if (key == GLFW_KEY_J)
		//{
		//	playerSphere->velocity += glm::(vec3*0.1f)(1.0f, 0.0f, 0.0f);
		//}
		//if (key == GLFW_KEY_L)
		//{
		//	playerSphere->velocity += glm::vec3(-1.0f, 0.0f, 0.0f);
		//}

		//if (key == GLFW_KEY_U)
		//{
		//	playerSphere->velocity += glm::vec3(0.0f, 1.0f, 0.0f);
		//}
		//if (key == GLFW_KEY_O)
		//{
		//	playerSphere->velocity += glm::vec3(0.0f, -1.0f, 0.0f);
		//}

		//if (key == GLFW_KEY_I)
		//{
		//	playerSphere->velocity += glm::vec3(0.0f, 0.0f, 1.0f);
		//}
		//if (key == GLFW_KEY_K)
		//{
		//	playerSphere->velocity += glm::vec3(0.0f, 0.0f, -1.0f);
		//}
		// </Sphere Movement> ******************************************************