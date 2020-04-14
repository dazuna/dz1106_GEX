#pragma once
#include "cGameObject.h"

struct InstanceRenderer
{
	// object to render multiple times! :D
	cGameObject* gameObj;
	/* states: new, ready2Draw*/
	std::string status = "new";
	// world matrices
	std::vector<glm::mat4> vecWMs;
	// map of instance renderers
	static std::map<std::string,InstanceRenderer*> mapIR;

	void setVAOVariables();
	void drawIR();
};

