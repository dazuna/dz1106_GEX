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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	const float CAMERASPEED = 2.0f;

	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{

		// Move the camera (A & D for left and right, along the x axis)
		if (key == GLFW_KEY_A)
		{
			cameraEye.x -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_D)
		{
			cameraEye.x += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (Q & E for up and down, along the y axis)
		if (key == GLFW_KEY_Q)
		{
			cameraEye.y -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_E)
		{
			cameraEye.y += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (W & S for towards and away, along the z axis)
		if (key == GLFW_KEY_W)
		{
			cameraEye.z -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_S)
		{
			cameraEye.z += CAMERASPEED;		// Move the camera +0.01f units
		}

		//if ( key == GLFW_KEY_B )
		//{ 
		//	// Shoot a bullet from the pirate ship
		//	cGameObject* pShip = pFindObjectByFriendlyNameMap("PirateShip");
		//	// Find the sphere#2
		//	cGameObject* pBall = pFindObjectByFriendlyNameMap("Sphere#2");
		//	// Set the location velocity for sphere#2
		//	pBall->positionXYZ = pShip->positionXYZ;
		//	pBall->inverseMass = 1.0f;		// So it's updated
		//	pBall->velocity = glm::vec3( 15.0f, 20.0f, 0.0f );
		//	pBall->accel = glm::vec3(0.0f,0.0f,0.0f);
		//	pBall->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		//}//if ( key == GLFW_KEY_B )

		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			selectedLight++;
			if (selectedLight == ::g_map_pLights.end()){
				selectedLight = ::g_map_pLights.begin();
			}	
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			system("cls");
			std::ostringstream tempSS;
			tempSS << "lightname: " << selectedLight->second.getName()
				<< " x: " << selectedLight->second.positionXYZ.x
				<< " y: " << selectedLight->second.positionXYZ.y
				<< " z: " << selectedLight->second.positionXYZ.z;
			console = tempSS.str();
			std::cout << console << std::endl;
		}

	}

	if (isShiftKeyDownByAlone(mods))
	{
		// move the light
		if (key == GLFW_KEY_A)
		{
			selectedLight->second.positionXYZ.x -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_D)
		{
			selectedLight->second.positionXYZ.x += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (Q & E for up and down, along the y axis)
		if (key == GLFW_KEY_Q)
		{
			selectedLight->second.positionXYZ.y -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_E)
		{
			selectedLight->second.positionXYZ.y += CAMERASPEED;		// Move the camera +0.01f units
		}

		// Move the camera (W & S for towards and away, along the z axis)
		if (key == GLFW_KEY_W)
		{
			selectedLight->second.positionXYZ.z -= CAMERASPEED;		// Move the camera -0.01f units
		}
		if (key == GLFW_KEY_S)
		{
			selectedLight->second.positionXYZ.z += CAMERASPEED;		// Move the camera +0.01f units
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