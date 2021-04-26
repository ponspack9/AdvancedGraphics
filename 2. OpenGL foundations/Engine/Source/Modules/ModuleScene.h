#pragma once
#include "Module.h"

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene();
	~ModuleScene();

public:
	std::vector<GameObject*> sceneObjects;
};

extern ModuleScene* M_Scene;