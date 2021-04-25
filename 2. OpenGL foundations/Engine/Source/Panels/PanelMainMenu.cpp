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
	ImGui::EndMainMenuBar();
}
