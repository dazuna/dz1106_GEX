#include "JSONLoader.h"

using json = nlohmann::json;
std::string LIGHT_JSON  = "./configFiles/lights.json";
std::string GAMEOBJECTS_JSON = "./configFiles/gameObjects.json";

bool JSONLoadMeshes(std::map<std::string, cMesh*>* g_map_Mesh, cModelLoader* pTheModelLoader)
{
	std::cout << "loading meshes...";
	std::ifstream inFile(GAMEOBJECTS_JSON.c_str());
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

bool JSONLoadLights(std::map<std::string, cLight>* g_map_pLights, GLuint shadProgID)
{
	std::cout << "loading lights...";
	std::ifstream inFile(LIGHT_JSON.c_str());
	json jsonArray;
	int index = 0;
	inFile >> jsonArray;

	for (index = 0; index < jsonArray.size(); index++)
	{
		std::string name = jsonArray[index]["name"];
		glm::vec3 posXYZ = glm::vec3(jsonArray[index]["position"][0],
			jsonArray[index]["position"][1],
			jsonArray[index]["position"][2]);
		float LinearAtten = jsonArray[index]["LinearAtten"];
        float QuadraticAtten = jsonArray[index]["QuadraticAtten"];
		cLight tempLight("theLights", name, shadProgID, index);
		//::g_map_pLights["light0"] = &light0;
		g_map_pLights->insert({ name.c_str(),tempLight });
	}
	//std::cout << j;
	std::cout << "[OK]\n" << index << " lights loaded" << std::endl;
	return true;
}

bool JSONLoadGameObjects(
	std::map<std::string, cGameObject*>* g_map_GameObjects)
{
	std::cout << "loading objects...";
	std::ifstream inFile(GAMEOBJECTS_JSON.c_str());
	json jsonArray;
	int index = 0;
	inFile >> jsonArray;

	for (index = 0; index < jsonArray.size(); index++)
	{
		std::string friendlyName = jsonArray[index]["friendlyName"];
		std::string meshName = jsonArray[index]["meshName"];
		glm::vec3 positionXYZ = glm::vec3(jsonArray[index]["positionXYZ"][0],
									jsonArray[index]["positionXYZ"][1],
									jsonArray[index]["positionXYZ"][2]);
		glm::vec3 rotationXYZ = glm::vec3(jsonArray[index]["rotationXYZ"][0],
									jsonArray[index]["rotationXYZ"][1],
									jsonArray[index]["rotationXYZ"][2]);
		float scale = jsonArray[index]["scale"];
		glm::vec4 objectColourRGBA = glm::vec4(jsonArray[index]["objectColourRGBA"][0],
									jsonArray[index]["objectColourRGBA"][1],
									jsonArray[index]["objectColourRGBA"][2],
									jsonArray[index]["objectColourRGBA"][3]);
		glm::vec4 diffuseColour = glm::vec4(jsonArray[index]["diffuseColour"][0],
									jsonArray[index]["diffuseColour"][1],
									jsonArray[index]["diffuseColour"][2],
									jsonArray[index]["diffuseColour"][3]);
		glm::vec4 specularColour = glm::vec4(jsonArray[index]["specularColour"][0],
									jsonArray[index]["specularColour"][1],
									jsonArray[index]["specularColour"][2],
									jsonArray[index]["specularColour"][3]);
		glm::vec3 velocity = glm::vec3(jsonArray[index]["velocity"][0],
									jsonArray[index]["velocity"][1],
									jsonArray[index]["velocity"][2]);
		glm::vec3 accel = glm::vec3(jsonArray[index]["accel"][0],
									jsonArray[index]["accel"][1],
									jsonArray[index]["accel"][2]);
		float inverseMass = jsonArray[index]["inverseMass"];
		int physicsShapeType = jsonArray[index]["physicsShapeType"];
		bool isWireframe = jsonArray[index]["isWireframe"];
		glm::vec4 debugColour = glm::vec4(jsonArray[index]["debugColour"][0],
									jsonArray[index]["debugColour"][1],
									jsonArray[index]["debugColour"][2],
									jsonArray[index]["debugColour"][3]);
		bool isVisible = jsonArray[index]["isVisible"];
		cGameObject* tempGameObject = new cGameObject();
		tempGameObject->friendlyName = friendlyName;
		tempGameObject->meshName = meshName;
		tempGameObject->positionXYZ = positionXYZ;
		tempGameObject->rotationXYZ = rotationXYZ;
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
	return true;
}

bool loadMeshToGPU(cVAOManager* pTheVAOManager,
	std::map<std::string, cMesh*>* g_map_Mesh,
	std::map<std::string, cGameObject*>* g_map_GameObjects,
	GLuint shaderProgID	)
{
	for (std::map<std::string, cGameObject*>::iterator itGO = g_map_GameObjects->begin();
		itGO != g_map_GameObjects->end();
		itGO++)
	{
		sModelDrawInfo drawInfo;
		pTheVAOManager->LoadModelIntoVAO(itGO->second->meshName,*(g_map_Mesh->at(itGO->second->meshName.c_str())), drawInfo, shaderProgID);

	}//for (int index...
	return true;
}