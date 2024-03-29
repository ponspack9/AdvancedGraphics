#pragma once
#include "Module.h"

#define WINDOW_TITLE  "Oscar Pons & David Tello - AGP"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

class Panel;

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	~ModuleWindow();

	bool Init() override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	GLFWwindow* window = nullptr;
	bool mouse_click = false;

private:
	f64 lastFrameTime = 0;
};

extern ModuleWindow* M_Window;