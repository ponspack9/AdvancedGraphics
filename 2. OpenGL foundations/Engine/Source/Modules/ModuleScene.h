#pragma once
#include "Module.h"

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init() override;

public:
	Camera* camera = nullptr;
	std::vector<Model*> models;
	std::vector<Light*> lights;
};

extern ModuleScene* M_Scene;