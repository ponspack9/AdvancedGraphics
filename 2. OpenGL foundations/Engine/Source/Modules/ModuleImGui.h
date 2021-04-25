#pragma once
#include "Module.h"

class Panel;

class ModuleImGui : public Module
{
public:
	ModuleImGui();
	~ModuleImGui();

	bool Init() override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	void DockSpace() const;

	void Draw();

	std::vector<Panel*> panels;

};

extern ModuleImGui* M_Gui;
