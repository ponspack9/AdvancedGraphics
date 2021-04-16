#include "pch.h"
#include "ModuleWindow.h"
#include <PanelInfo.h>

ModuleWindow* M_Window = nullptr;

ModuleWindow::ModuleWindow()
{
	this->name = "ModuleWindow";

	LOG_DEBUG("Created module [{0}]", name);
}
ModuleWindow::~ModuleWindow()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}
bool ModuleWindow::Init()
{

	return true;
}

bool ModuleWindow::Start()
{
	return true;
}

bool ModuleWindow::PreUpdate(float dt)
{
	return true;
}

bool ModuleWindow::Update(float dt)
{
	return true;
}

bool ModuleWindow::PostUpdate(float dt)
{
	return true;
}



bool ModuleWindow::CleanUp()
{
	return true;
}


