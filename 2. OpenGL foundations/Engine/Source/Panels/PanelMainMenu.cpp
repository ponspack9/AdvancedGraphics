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


	char fps[64];
	sprintf_s(fps, 64, "FPS: %f", 1.0f/App->deltaTime);
	ImGui::Text(fps);
	ImGui::EndMainMenuBar();
}
