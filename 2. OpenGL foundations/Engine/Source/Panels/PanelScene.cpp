#include "pch.h"
#include "PanelScene.h"
#include <ModuleResources.h>

PanelScene::PanelScene()
{
}
PanelScene::~PanelScene()
{
}
void PanelScene::Draw()
{
	ImGui::Begin("Scene");

	ImGui::Image((ImTextureID)M_Resources->textures[0].handle, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
	ImGui::End();
}