#pragma once
#include <map>

#include "cGameObject.h"
#include "cLight.h"

typedef std::map<std::string,cGameObject*> mapGO;
typedef std::map<std::string, cLight*> mapLights;
