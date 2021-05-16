#include "pch.h"
#include "ModuleScene.h"
#include <ModuleResources.h>

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
	vec3 pos	= glm::vec3(9.0f, 0.0f, 14.0f);
	vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	camera = new Camera(pos, center, 5.0f, FOV, App->displaySize.x / App->displaySize.y, NEAR_PLANE, FAR_PLANE);

	// Set Models
	Model* gameObject = M_Resources->LoadModel("Patrick/Patrick.obj");
	gameObject->position = { 0,0,0 };
	models.push_back(gameObject);
	gameObject->index = "0";

	Model* patricio2 = new Model(*gameObject);
	patricio2->position = { 3,0,-3 };
	patricio2->index = "1";
	models.push_back(patricio2);
	//models.push_back(gameObject);
	
	
	//Model* plane = new Model(*M_Resources->plane);
	//plane->position = { 0,-4,0 };
	//plane->scale = {10,10,10};
	//plane->index = "3";
	//models.push_back(plane);


	// Set Lights
	//...

	return true;
}
