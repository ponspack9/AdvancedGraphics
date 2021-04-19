#include "pch.h"

#include "Application.h"
// Modules
#include <ModuleImGui.h>
#include <ModuleWindow.h>
// END INCLUDE MODULES

Application* App = nullptr;

Application::Application()
{
	App = this;
	M_Window = new ModuleWindow();
	//M_Gui = new ModuleImGui();
	// END NEW MODULES

	modules.push_back(M_Window);
	modules.push_back(M_Gui);
	// END NEW REFERENCE MODULES
}

Application::~Application()
{
	for (Module* module : modules)
	{
		delete module;
	}
	modules.clear();
}

bool Application::Init()
{
	for (Module* module : modules)
	{
		if (!module->Init())
		{
			LOG_ERROR("INIT ERROR returned by module [{0}]", module->GetName());
			return false;
		}
	}
	return true;
}

bool Application::Start()
{
	for (Module* module : modules)
	{
		if (!module->Start())
		{
			LOG_ERROR("START ERROR returned by module [{0}]", module->GetName());
			return false;
		}
	}
	return true;
}

// Called by entry point, main application loop
bool Application::MainUpdate()
{
	bool status = PreUpdate();

	if (status)
		status = Update();

	if (status)
		status = PostUpdate();

	return status;
}

bool Application::PreUpdate()
{
	bool status = false;
	for (Module* module : modules)
	{
		status = module->PreUpdate(deltaTime);
		if (!status)
		{
			LOG_ERROR("PREUPDATE_STOP returned by module [{0}]", module->GetName());
			break;
		}
	}
	return status;
}

bool Application::Update()
{
	bool status = false;
	for (Module* module : modules)
	{
		status = module->Update(deltaTime);
		if (!status)
		{
			LOG_ERROR("UPDATE_STOP returned by module [{0}]", module->GetName());
			break;
		}
	}
	return status;
}

bool Application::PostUpdate()
{
	bool status = false;
	for (Module* module : modules)
	{
		status = module->PostUpdate(deltaTime);
		if (!status)
		{
			LOG_ERROR("POSTUPDATE_STOP returned by module [{0}]", module->GetName());
			break;
		}break;

	}
	return status;
}

bool Application::CleanUp()
{
	for (Module* module : modules)
	{
		if (!module->CleanUp())
		{
			LOG_ERROR("START ERROR returned by module [{0}]", module->GetName());
			return false;
		}
	}
	return true;
}

