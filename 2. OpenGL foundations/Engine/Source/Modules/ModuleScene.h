#pragma once
#include "Module.h"

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene();
	~ModuleScene();

	bool Init() override;

public:
	std::vector<GameObject*> sceneObjects;
	GameObject* selected = nullptr;
	GameObject* camera = nullptr;
};

extern ModuleScene* M_Scene;