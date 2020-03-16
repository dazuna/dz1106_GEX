#include "cStencilScene.h"

bool cStencilScene::init()
{
	std::cout << "pre stencil objects: " << preStencilObjs.size() << std::endl;
	std::cout << "stencil object: " << theStencilObj->friendlyName << std::endl;
	std::cout << "pre stencil objects: " << postStencilObjs.size() << std::endl;
	return true;
}

