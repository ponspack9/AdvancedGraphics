#pragma once
#include "Module.h"

class GameObject;

class ModuleScene : public Module
{

public:
	std::vector<GameObject*> sceneObjects;
};

