#pragma once
#include "cScene.h"

class cStencilScene : public cScene
{
public:
	std::vector<cGameObject*> preStencilObjs;
	cGameObject* theStencilObj;
	std::vector<cGameObject*> postStencilObjs;
	
	bool init();
};