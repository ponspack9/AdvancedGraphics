#include "pch.h"
#include "ModuleScene.h"
#include <ModuleResources.h>
#include <GameObjects/GameObject.h>

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
	GameObject* gameObject = new GameObject();
	sceneObjects.push_back(gameObject);
	
	gameObject->components.push_back((Component*)M_Resources->models[0]);

	selected = gameObject;

	return true;
}