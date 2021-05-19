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
	camera = new Camera(pos, center, FOV, App->displaySize.x / App->displaySize.y, NEAR_PLANE, FAR_PLANE);

	// Set Models
	Model* gameObject = M_Resources->LoadModel("Patrick/Patrick.obj");
	gameObject->position = { -5,0,-1 };
	models.push_back(gameObject);
	gameObject->index = "0";

	Model* patricio2 = new Model(*gameObject);
	patricio2->position = { 5,0,-1 };
	patricio2->index = "1";
	models.push_back(patricio2);

	patricio2 = new Model(*gameObject);
	patricio2->position = { 0,0,5 };
	patricio2->index = "2";
	models.push_back(patricio2);

	Model* plane = new Model(*M_Resources->plane);
	plane->position = { 0,-4,0 };
	plane->scale = { 10,10,10 };
	plane->index = "55";
	models.push_back(plane);

	// Directional Lights
	Light* dlight1 = new Light(Light::LightType::DIRECTIONAL, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, 3.0f));
	Light* dlight2 = new Light(Light::LightType::DIRECTIONAL, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-3.0f, 2.0f, -3.0f));
	lights.push_back(dlight1);
	lights.push_back(dlight2);

	// Point Lights
	Light* plight1 = new Light(Light::LightType::POINT, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.0f, 5.0f, 3.0f),	1.0f);
	Light* plight2 = new Light(Light::LightType::POINT, glm::vec3(0.7f, 0.5f, 0.2f), glm::vec3(0.0f), glm::vec3(2.0f, 0.0f, 5.0f),	3.6f);
	Light* plight3 = new Light(Light::LightType::POINT, glm::vec3(1.0f, 0.8f, 0.8f), glm::vec3(0.0f), glm::vec3(-4.0f, 5.0f, 9.0f), 5.0f);
	Light* plight4 = new Light(Light::LightType::POINT, glm::vec3(0.0f, 0.5f, 0.9f), glm::vec3(0.0f), glm::vec3(-1.0f, 5.0f, 2.0f), 0.5f);
	Light* plight5 = new Light(Light::LightType::POINT, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 2.0f, -3.0f), 2.0f);
	lights.push_back(plight1);
	lights.push_back(plight2);
	lights.push_back(plight3);
	lights.push_back(plight4);
	lights.push_back(plight5);

	return true;
}
