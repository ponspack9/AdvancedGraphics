#pragma once
#include "Module.h"

//class Camera;
//class Light;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init() override;

public:
	Camera* camera = nullptr;
	std::vector<Light*> lights;
	std::vector<Model*> models;
};

extern ModuleScene* M_Scene;