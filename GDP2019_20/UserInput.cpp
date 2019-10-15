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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::mat4 matRotX,matRotY;
	glm::vec3 invVisVec;
	const float CAMERASPEED = 2.0f;
	const float DEGREESOFROTATION = 3.0f;

	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{
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
	}

	if (isShiftKeyDownByAlone(mods))
	{
		// move the light
		if (key == GLFW_KEY_A)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	selectedGameObject->second->positionXYZ.x -= CAMERASPEED; break;
			case selectedType::LIGHT:		selectedLight->second.positionXYZ.x -= CAMERASPEED;	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_D)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	selectedGameObject->second->positionXYZ.x += CAMERASPEED; break;
			case selectedType::LIGHT:		selectedLight->second.positionXYZ.x += CAMERASPEED;	break;
			case selectedType::SOUND:		break;
			}
		}

		// Move the camera (Q & E for up and down, along the y axis)
		if (key == GLFW_KEY_Q)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	selectedGameObject->second->positionXYZ.y -= CAMERASPEED; break;
			case selectedType::LIGHT:		selectedLight->second.positionXYZ.y -= CAMERASPEED;	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_E)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	selectedGameObject->second->positionXYZ.y += CAMERASPEED; break;
			case selectedType::LIGHT:		selectedLight->second.positionXYZ.y += CAMERASPEED;	break;
			case selectedType::SOUND:		break;
			}
		}

		// Move the camera (W & S for towards and away, along the z axis)
		if (key == GLFW_KEY_W)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	selectedGameObject->second->positionXYZ.z -= CAMERASPEED; break;
			case selectedType::LIGHT:		selectedLight->second.positionXYZ.z -= CAMERASPEED;	break;
			case selectedType::SOUND:		break;
			}
		}
		if (key == GLFW_KEY_S)
		{
			switch (cursorType)
			{
			case selectedType::GAMEOBJECT:	selectedGameObject->second->positionXYZ.z += CAMERASPEED; break;
			case selectedType::LIGHT:		selectedLight->second.positionXYZ.z += CAMERASPEED;	break;
			case selectedType::SOUND:		break;
			}
		}
		//if (key == GLFW_KEY_1)
		//{
		//	selectedLight->second.ConstAtten *= 0.99f;			// 99% of what it was
		//}
		//if (key == GLFW_KEY_2)
		//{
		//	selectedLight->second.ConstAtten *= 1.01f;			// 1% more of what it was
		//}		
		if (key == GLFW_KEY_3)
		{
			selectedLight->second.LinearAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_4)
		{
			selectedLight->second.LinearAtten *= 1.01f;			// 1% more of what it was
		}
		if (key == GLFW_KEY_5)
		{
			selectedLight->second.QuadraticAtten *= 0.99f;			// 99% of what it was
		}
		if (key == GLFW_KEY_6)
		{
			selectedLight->second.QuadraticAtten *= 1.01f;			// 1% more of what it was
		}
		//if (key == GLFW_KEY_V)
		//{
		//	selectedLight->second.SpotInnerAngle -= 0.1f;
		//}
		//if (key == GLFW_KEY_B)
		//{
		//	selectedLight->second.SpotInnerAngle += 0.1f;
		//}
		//if (key == GLFW_KEY_N)
		//{
		//	selectedLight->second.SpotOuterAngle -= 0.1f;
		//}
		//if (key == GLFW_KEY_M)
		//{
		//	selectedLight->second.SpotOuterAngle += 0.1f;
		//}
		if (key == GLFW_KEY_9)
		{
			bLightDebugSheresOn = false;			
		}
		if (key == GLFW_KEY_0)
		{
			bLightDebugSheresOn = true; 
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
		//if (key == GLFW_KEY_R && action == GLFW_PRESS)
		//{
		//	::g_map_GameObjects.clear();
		//	selectedGameObject = ::g_map_GameObjects.begin();
		//	JSONLoadGameObjects(&::g_map_GameObjects);
		//}

	}//if (isShiftKeyDownByAlone(mods))

	if (isCtrlKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			JSONSaveLights(&::g_map_pLights);
			JSONSaveGameObjects(&::g_map_GameObjects);
		}
	}

	//// Moving the pirate ship in a certain direction
	//if (isCtrlKeyDownByAlone(mods))
	//{
	//	const float SHIP_SPEED_CHANGE = 0.01f;
	//	const float SHIP_ANGLE_CHANGE = 0.01f;

	//	cGameObject* pShip = pFindObjectByFriendlyName("PirateShip");
	//	// Turn the ship around
	//	if (key == GLFW_KEY_A)
	//	{	// Left
	//		pShip->HACK_AngleAroundYAxis -= SHIP_ANGLE_CHANGE;
	//		pShip->rotationXYZ.y = pShip->HACK_AngleAroundYAxis;
	//	}
	//	if (key == GLFW_KEY_D)
	//	{	// Right
	//		pShip->HACK_AngleAroundYAxis += SHIP_ANGLE_CHANGE;
	//		pShip->rotationXYZ.y = pShip->HACK_AngleAroundYAxis;
	//	}
	//	if (key == GLFW_KEY_W)
	//	{	// Faster
	//		pShip->HACK_speed += SHIP_SPEED_CHANGE;
	//	}
	//	if (key == GLFW_KEY_S)
	//	{	// Slower
	//		pShip->HACK_speed -= SHIP_SPEED_CHANGE;
	//	}
	//}

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
	//// Ignore other keys and see if the shift key is down
	//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT){}
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