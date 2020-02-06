#include "JSONLoader.h"

using json = nlohmann::json;

std::string JSONLoader::light_json = "./configFiles/lights.json";
std::string JSONLoader::gameobjects_json = "./configFiles/gameObjects.json";
std::string JSONLoader::bkp_light_json = "./configFiles/bkplights.json";
std::string JSONLoader::bkp_gameobjects_json = "./configFiles/bkpgameObjects.json";

bool JSONLoader::JSONLoadMeshes(std::map<std::string, cMesh*>* g_map_Mesh, cModelLoader* pTheModelLoader)
{
	std::cout << "loading meshes...";
	std::ifstream inFile(gameobjects_json.c_str());
	json jsonArray;
	int index = 0;
	inFile >> jsonArray;

	for (index = 0; index < jsonArray.size(); index++)
	{
		std::string name = jsonArray[index]["meshName"];
		std::string location = jsonArray[index]["meshURL"];
		cMesh* tempMesh = new cMesh();
		pTheModelLoader->LoadPlyModel(location.c_str(), *tempMesh);
		g_map_Mesh->insert({ name.c_str(),tempMesh });
	}
	//std::cout << j;
	std::cout << "[OK]\n" << index << " models loaded" << std::endl;
	return true;
}

bool JSONLoader::JSONLoadLights(std::map<std::string, cLight>* g_map_pLights, GLuint shadProgID)
{
	std::cout << "loading lights...";
	std::ifstream inFile(light_json.c_str());
	std::ofstream outFile(bkp_light_json.c_str());
	json jsonArray;
	int index = 0;
	inFile >> jsonArray;

	for (index = 0; index < jsonArray.size(); index++)
	{
		std::string name = jsonArray[index]["name"];
		glm::vec3 positionXYZ = glm::vec3(
			jsonArray[index]["positionXYZ"][0],
			jsonArray[index]["positionXYZ"][1],
			jsonArray[index]["positionXYZ"][2]);		
		float LinearAtten = jsonArray[index]["LinearAtten"];
        float QuadraticAtten = jsonArray[index]["QuadraticAtten"];
		glm::vec3 diffuse = glm::vec3(
			jsonArray[index]["diffuse"][0],
			jsonArray[index]["diffuse"][1],
			jsonArray[index]["diffuse"][2]);
		glm::vec3 specular = glm::vec3(
			jsonArray[index]["specular"][0],
			jsonArray[index]["specular"][1],
			jsonArray[index]["specular"][2]);	
		glm::vec3 direction = glm::vec3(
			jsonArray[index]["direction"][0],
			jsonArray[index]["direction"][1],
			jsonArray[index]["direction"][2]);
		float type = jsonArray[index]["type"];
		float innerAngle = jsonArray[index]["innerAngle"];
		float outerAngle = jsonArray[index]["outerAngle"];
		float lightW = jsonArray[index]["lightW"];
		float lightSwitch = jsonArray[index]["lightSwitch"];
		cLight tempLight("theLights", name, shadProgID, index);
		// load values to tempLight
		tempLight.positionXYZ = positionXYZ;
		tempLight.LinearAtten = LinearAtten;
		tempLight.QuadraticAtten = QuadraticAtten;
		tempLight.diffuse = diffuse;
		tempLight.specular = specular;
		tempLight.setAT(direction);
		tempLight.type = type;
		tempLight.innerAngle = innerAngle;
		tempLight.outerAngle = outerAngle;
		tempLight.lightW = lightW;
		tempLight.lightSwitch = lightSwitch;
		//load to global map ::g_map_pLights["light0"] = &light0;
		g_map_pLights->insert({ name.c_str(),tempLight });
	}
	//std::cout << j;
	std::cout << "[OK]\n" << index << " lights loaded" << std::endl;
	outFile << jsonArray;
	return true;
}

bool JSONLoader::JSONLoadGameObjects(
	std::map<std::string, cGameObject*>* g_map_GameObjects)
{
	std::cout << "loading objects...";
	std::ifstream inFile(gameobjects_json.c_str());
	std::ofstream outFile("./configFiles/log.txt");
	std::ofstream outFile2(bkp_gameobjects_json.c_str());
	json jsonArray;
	int index = 0;
	inFile >> jsonArray;

	for (index = 0; index < jsonArray.size(); index++)
	{
		std::string friendlyName = jsonArray[index]["friendlyName"];
		outFile << "friendlyName: " << friendlyName << std::endl;
		std::string meshName = jsonArray[index]["meshName"];
		outFile << "meshName: " << meshName << std::endl;
		std::string meshURL = jsonArray[index]["meshURL"];
		outFile << "meshURL: " << meshURL << std::endl;
		glm::vec3 positionXYZ = glm::vec3(
			jsonArray[index]["positionXYZ"][0],
			jsonArray[index]["positionXYZ"][1],
			jsonArray[index]["positionXYZ"][2]);
		outFile << "positionXYZ: " << glm::to_string(positionXYZ) << std::endl;
		glm::vec3 rotationXYZ = glm::vec3(
			jsonArray[index]["rotationXYZ"][0],
			jsonArray[index]["rotationXYZ"][1],
			jsonArray[index]["rotationXYZ"][2]);
		outFile << "rotationXYZ: " << glm::to_string(rotationXYZ) << std::endl;
		float scale = jsonArray[index]["scale"];
		glm::vec4 objectColourRGBA = glm::vec4(
			jsonArray[index]["objectColourRGBA"][0],
			jsonArray[index]["objectColourRGBA"][1],
			jsonArray[index]["objectColourRGBA"][2],
			jsonArray[index]["objectColourRGBA"][3]);
		outFile << "objectColourRGBA: " << glm::to_string(objectColourRGBA) << std::endl;
		glm::vec4 diffuseColour = glm::vec4(
			jsonArray[index]["diffuseColour"][0],
			jsonArray[index]["diffuseColour"][1],
			jsonArray[index]["diffuseColour"][2],
			jsonArray[index]["diffuseColour"][3]);
		outFile << "diffuseColour: " << glm::to_string(diffuseColour) << std::endl;
		glm::vec4 specularColour = glm::vec4(
			jsonArray[index]["specularColour"][0],
			jsonArray[index]["specularColour"][1],
			jsonArray[index]["specularColour"][2],
			jsonArray[index]["specularColour"][3]);
		outFile << "specularColour: " << glm::to_string(specularColour) << std::endl;
		glm::vec3 velocity = glm::vec3(
			jsonArray[index]["velocity"][0],
			jsonArray[index]["velocity"][1],
			jsonArray[index]["velocity"][2]);
		outFile << "velocity: " << glm::to_string(velocity) << std::endl;
		glm::vec3 accel = glm::vec3(
			jsonArray[index]["accel"][0],
			jsonArray[index]["accel"][1],
			jsonArray[index]["accel"][2]);
		outFile << "accel: " << glm::to_string(accel) << std::endl;
		float inverseMass = jsonArray[index]["inverseMass"];

		int physicsShapeType = jsonArray[index]["physicsShapeType"];
		bool isWireframe = jsonArray[index]["isWireframe"];
		glm::vec4 debugColour = glm::vec4(
			jsonArray[index]["debugColour"][0],
			jsonArray[index]["debugColour"][1],
			jsonArray[index]["debugColour"][2],
			jsonArray[index]["debugColour"][3]);
		outFile << "debugColour: " << glm::to_string(debugColour) << std::endl;
		bool isVisible = jsonArray[index]["isVisible"];

		cGameObject* tempGameObject = new cGameObject();
		if (jsonArray[index].find("texture") != jsonArray[index].end())
		{
			for (int i = 0; i < jsonArray[index]["texture"].size(); i++)
			{
				tempGameObject->textures.push_back(jsonArray[index]["texture"][i]);
			}
		}
		if (jsonArray[index].find("textureRatio") != jsonArray[index].end())
		{
			for (int i = 0; i < jsonArray[index]["textureRatio"].size(); i++)
			{
				tempGameObject->textureRatio.push_back(jsonArray[index]["textureRatio"][i]);
			}
		}
		if (jsonArray[index].find("alphaTransparency") != jsonArray[index].end())
		{
			tempGameObject->alphaTransparency = jsonArray[index]["alphaTransparency"];
		}
		tempGameObject->friendlyName = friendlyName;
		tempGameObject->meshName = meshName;
		tempGameObject->meshURL = meshURL;
		tempGameObject->positionXYZ = positionXYZ;
		tempGameObject->setOrientation(rotationXYZ);
		tempGameObject->scale = scale;
		tempGameObject->objectColourRGBA = objectColourRGBA;
		tempGameObject->diffuseColour = diffuseColour;
		tempGameObject->specularColour = specularColour;
		tempGameObject->velocity = velocity;
		tempGameObject->accel = accel;
		tempGameObject->inverseMass = inverseMass;
		tempGameObject->physicsShapeType = (eShapeTypes)physicsShapeType;
		tempGameObject->isWireframe = isWireframe;
		tempGameObject->debugColour = debugColour;
		tempGameObject->isVisible = isVisible;
		g_map_GameObjects->insert({ friendlyName.c_str(),tempGameObject });
	}
	//std::cout << j;
	std::cout << "[OK]\n" << index << " objects loaded" << std::endl;
	outFile2 << jsonArray;
	JSONLoadTextures();
	return true;
}

bool JSONLoader::loadMeshToGPU(cVAOManager* pTheVAOManager,
	std::map<std::string, cMesh*>* g_map_Mesh,
	std::map<std::string, cGameObject*>* g_map_GameObjects,
	GLuint shaderProgID	)
{
	for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects->begin();
		itGO != g_map_GameObjects->end();
		itGO++)
	{
		sModelDrawInfo drawInfo;
		pTheVAOManager->LoadModelIntoVAO(
			itGO->second->meshName,
			*(g_map_Mesh->at(itGO->second->meshName.c_str())),
			drawInfo, shaderProgID);

	}//for (int index...
	return true;
}

bool JSONLoader::JSONSaveLights(std::map<std::string, cLight>* g_map_pLights)
{
	std::cout << "saving lights...";
	std::ofstream outFile("./configFiles/lights.json");
	std::map<std::string, cLight>::iterator index = g_map_pLights->begin();
	int x = 0;
	json jsonArray;

	for (index; index != g_map_pLights->end(); index++, x++)
	{
		json jsonObject;
		jsonObject["name"] = index->second.getName();
		jsonObject["positionXYZ"][0] = index->second.positionXYZ.x;
		jsonObject["positionXYZ"][1] = index->second.positionXYZ.y;
		jsonObject["positionXYZ"][2] = index->second.positionXYZ.z;
		jsonObject["LinearAtten"] = index->second.LinearAtten;
		jsonObject["QuadraticAtten"] = index->second.QuadraticAtten;
		jsonObject["diffuse"][0] = index->second.diffuse.x;
		jsonObject["diffuse"][1] = index->second.diffuse.y;
		jsonObject["diffuse"][2] = index->second.diffuse.z;
		jsonObject["specular"][0] = index->second.specular.x;
		jsonObject["specular"][1] = index->second.specular.y;
		jsonObject["specular"][2] = index->second.specular.z;
		jsonObject["direction"][0] = index->second.getCurrentAT().x;
		jsonObject["direction"][1] = index->second.getCurrentAT().y;
		jsonObject["direction"][2] = index->second.getCurrentAT().z;
		jsonObject["type"] = index->second.type;
		jsonObject["innerAngle"] = index->second.innerAngle;
		jsonObject["outerAngle"] = index->second.outerAngle;
		jsonObject["lightW"] = index->second.lightW;
		jsonObject["lightSwitch"] = index->second.lightSwitch;
		jsonArray[x] = jsonObject;
	}
	//std::cout << jsonArray;
	outFile << jsonArray;
	std::cout << "[OK]\n" << x << " lights saved!" << std::endl;
	return true;
}

bool JSONLoader::JSONSaveGameObjects(std::map<std::string, cGameObject*>* g_map_GameObjects)
{
	std::cout << "saving gameObjects...";
	std::ofstream outFile("./configFiles/gameObjects.json");
	std::map<std::string, cGameObject*>::iterator index = g_map_GameObjects->begin();
	int x = 0;
	json jsonArray;

	for (index; index != g_map_GameObjects->end(); index++, x++)
	{
		json jsonObject;
		jsonObject["friendlyName"] = index->second->friendlyName;
		jsonObject["meshName"] = index->second->meshName;
		jsonObject["meshURL"] = index->second->meshURL;
		jsonObject["positionXYZ"][0] = index->second->positionXYZ.x;
		jsonObject["positionXYZ"][1] = index->second->positionXYZ.y;
		jsonObject["positionXYZ"][2] = index->second->positionXYZ.z;
		jsonObject["rotationXYZ"][0] = index->second->getEulerAngle().x;
		jsonObject["rotationXYZ"][1] = index->second->getEulerAngle().y;
		jsonObject["rotationXYZ"][2] = index->second->getEulerAngle().z;
		jsonObject["scale"] = index->second->scale;
		jsonObject["objectColourRGBA"][0] = index->second->objectColourRGBA.x;
		jsonObject["objectColourRGBA"][1] = index->second->objectColourRGBA.y;
		jsonObject["objectColourRGBA"][2] = index->second->objectColourRGBA.z;
		jsonObject["objectColourRGBA"][3] = index->second->objectColourRGBA.w;
		jsonObject["diffuseColour"][0] = index->second->diffuseColour.x;
		jsonObject["diffuseColour"][1] = index->second->diffuseColour.y;
		jsonObject["diffuseColour"][2] = index->second->diffuseColour.z;
		jsonObject["diffuseColour"][3] = index->second->diffuseColour.w;
		jsonObject["specularColour"][0] = index->second->specularColour.x;
		jsonObject["specularColour"][1] = index->second->specularColour.y;
		jsonObject["specularColour"][2] = index->second->specularColour.z;
		jsonObject["specularColour"][3] = index->second->specularColour.w;
		jsonObject["velocity"][0] = index->second->velocity.x;
		jsonObject["velocity"][1] = index->second->velocity.y;
		jsonObject["velocity"][2] = index->second->velocity.z;
		jsonObject["accel"][0] = index->second->accel.x;
		jsonObject["accel"][1] = index->second->accel.y;
		jsonObject["accel"][2] = index->second->accel.z;
		jsonObject["inverseMass"] = index->second->inverseMass;
		jsonObject["physicsShapeType"] = index->second->physicsShapeType;
		jsonObject["isWireframe"] = index->second->isWireframe;
		jsonObject["debugColour"][0] = index->second->debugColour.x;
		jsonObject["debugColour"][1] = index->second->debugColour.y;
		jsonObject["debugColour"][2] = index->second->debugColour.z;
		jsonObject["debugColour"][3] = index->second->debugColour.w;
		jsonObject["texture"][0] = index->second->textures[0];
		jsonObject["texture"][1] = index->second->textures[1];
		jsonObject["texture"][2] = index->second->textures[2];
		jsonObject["texture"][3] = index->second->textures[3];
		jsonObject["textureRatio"][0] = index->second->textureRatio[0];
		jsonObject["textureRatio"][1] = index->second->textureRatio[1];
		jsonObject["textureRatio"][2] = index->second->textureRatio[2];
		jsonObject["textureRatio"][3] = index->second->textureRatio[3];
		jsonObject["isVisible"] = index->second->isVisible;
		jsonObject["alphaTransparency"] = index->second->alphaTransparency;
		jsonArray[x] = jsonObject;
	}
	//std::cout << jsonArray;
	outFile << jsonArray;
	std::cout << "[OK]\n" << x << " lights saved!" << std::endl;
	return true;
}

bool JSONLoader::JSONLoadTextures()
{
	for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects.begin();
		itGO != ::g_map_GameObjects.end();
		itGO++)
	{
		for (int i = 0; i < itGO->second->NUMBEROFTEXTURES; i++)
		{
			if (itGO->second->textures[i] != "");
			{
				::pTextureManager->Create2DTextureFromBMPFile(itGO->second->textures[i].c_str(), true);
			}
		}
	}
	return true;
}



