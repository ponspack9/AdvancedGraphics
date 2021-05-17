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

	//Model* plane = new Model(*M_Resources->plane);
	//plane->position = { 0,-4,0 };
	//plane->scale = { 10,10,10 };
	//plane->index = "55";
	//models.push_back(plane);

	// Directional Lights
	DirectionalLight* light0 = new DirectionalLight();
	DirectionalLight* light1 = new DirectionalLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, 3.0f));
	DirectionalLight* light2 = new DirectionalLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(-3.0f, 2.0f, -3.0f), 0.8f);
	dirLights.push_back(light0);
	dirLights.push_back(light1);
	dirLights.push_back(light2);

	// Point Lights
	PointLight* plight0 = new PointLight(glm::vec3(1.0f, 0.0f, 0.8f), glm::vec3(10.0f, 0.0f, 5.0f));
	PointLight* plight1 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, 3.0f));
	PointLight* plight2 = new PointLight(glm::vec3(0.7f, 0.5f, 0.2f), glm::vec3(2.0f, 0.0f, 5.0f));
	PointLight* plight3 = new PointLight(glm::vec3(1.0f, 0.8f, 0.8f), glm::vec3(-4.0f, 5.0f, 9.0f));
	PointLight* plight4 = new PointLight(glm::vec3(0.0f, 0.5f, 0.9f), glm::vec3(-1.0f, 5.0f, 2.0f));
	PointLight* plight5 = new PointLight(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 2.0f, -3.0f));
	PointLight* plight6 = new PointLight(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
	PointLight* plight7 = new PointLight(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-2.0f, 1.0f, -3.0f));
	PointLight* plight8 = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 2.0f, 2.0f));
	PointLight* plight9 = new PointLight(glm::vec3(0.7f, 0.2f, 0.7f), glm::vec3(4.0f, 4.0f, 8.0f));
	pointLights.push_back(plight0);
	pointLights.push_back(plight1);
	pointLights.push_back(plight2);
	pointLights.push_back(plight3);
	pointLights.push_back(plight4);
	pointLights.push_back(plight5);
	pointLights.push_back(plight6);
	pointLights.push_back(plight7);
	pointLights.push_back(plight8);
	pointLights.push_back(plight9);

	return true;
}
