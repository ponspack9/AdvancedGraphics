#include "pch.h"
#include "PanelMainMenu.h"

PanelMainMenu::PanelMainMenu()
{
}
PanelMainMenu::~PanelMainMenu()
{
}
void PanelMainMenu::Draw()
{
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("File");
	ImGui::MenuItem("Edit");
	ImGui::MenuItem("Help");


	char fps[16];
	sprintf_s(fps, 16, "FPS: %f", 1.0f/App->deltaTime);
	ImGui::MenuItem(fps);
	ImGui::EndMainMenuBar();
}
