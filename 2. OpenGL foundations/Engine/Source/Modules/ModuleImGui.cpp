#include "pch.h"
#include "ModuleImGui.h"
#include <PanelInfo.h>

ModuleImGui* M_Gui = nullptr;

ModuleImGui::ModuleImGui()
{
	this->name = "ModuleImGui";

	panels.push_back(new PanelInfo());


	LOG_DEBUG("Created module [{0}]", name);
}
ModuleImGui::~ModuleImGui()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}
bool ModuleImGui::Init()
{

	return true;
}

bool ModuleImGui::Start()
{
	return true;
}

bool ModuleImGui::PreUpdate(float dt)
{
	return true;
}

bool ModuleImGui::Update(float dt)
{
	return true;
}

bool ModuleImGui::PostUpdate(float dt)
{
	return true;
}


void ModuleImGui::Draw()
{
	for (Panel* panel : panels)
	{
		panel->Draw();
	}
}

bool ModuleImGui::CleanUp()
{
	return true;
}


