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

	//Model* cube = M_Resources->LoadModel("Box/Cube.fbx");
	//cube->position = { 0,0,0 };
	//cube->scale = { 0.02f, 0.02f, 0.02f };
	//cube->index = "3";
	//cube->useReliefMap = true;
	//Material* material = M_Resources->materials[cube->materialIdx[0]];
	//material->bumpTexture = M_Resources->LoadTexture2D("Box/Cube/toy_box_disp.png");
	//models.push_back(cube);

	Model* plane = new Model(*M_Resources->plane);
	plane->position = { 0,-4,0 };
	plane->scale = { 10,10,10 };
	plane->index = "55";
	models.push_back(plane);

	// Directional Light
	dirLight = new DirectionalLight(glm::vec3(0.26f, 0.91f, 0.87f), glm::vec3(-2.0f, 5.0f, 3.0f));

	// Point Lights
	PointLight* plight1 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 5.0f, 3.0f), 1.0f);	pointLights.push_back(plight1);
	PointLight* plight2 = new PointLight(glm::vec3(0.7f, 0.5f, 0.2f), glm::vec3(2.0f, 0.0f, 5.0f), 3.6f);	pointLights.push_back(plight2);
	PointLight* plight3 = new PointLight(glm::vec3(1.0f, 0.8f, 0.8f), glm::vec3(-4.0f, 5.0f, 9.0f), 5.0f);	pointLights.push_back(plight3);
	PointLight* plight4 = new PointLight(glm::vec3(0.0f, 0.5f, 0.9f), glm::vec3(-1.0f, 5.0f, 2.0f), 0.5f);	pointLights.push_back(plight4);
	PointLight* plight5 = new PointLight(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 2.0f, -3.0f), 2.0f);	pointLights.push_back(plight5);

	return true;
}
