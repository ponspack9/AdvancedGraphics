#include "pch.h"
#include "ModuleScene.h"
#include <ModuleResources.h>

#define FOV 60
#define NEAR_PLANE 0.1
#define FAR_PLANE 10000

ModuleScene* M_Scene = nullptr;

ModuleScene::ModuleScene()
{
	this->name = "ModuleScene";
	LOG_DEBUG("Created module [{0}]", name);
}

ModuleScene::~ModuleScene()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}

bool ModuleScene::Init()
{
	// Set Camera
	vec3 pos	= glm::vec3(5.0f, 0.0f, 0.0f);
	vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	camera = new Camera(pos, center, 5.0f, FOV, App->displaySize.x / App->displaySize.y, NEAR_PLANE, FAR_PLANE);

	// Set Models
	Model* gameObject = M_Resources->LoadModel("Patrick/Patrick.obj");
	models.push_back(gameObject);

	// Set Lights
	//...

	return true;
}
