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
	DirectionalLight* dirLight = nullptr;
	std::vector<PointLight*> pointLights;
};

extern ModuleScene* M_Scene;