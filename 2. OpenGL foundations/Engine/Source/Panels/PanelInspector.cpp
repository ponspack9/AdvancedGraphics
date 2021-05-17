#include "pch.h"
#include "PanelInspector.h"
#include <ModuleScene.h>
#include <ModuleRenderer.h>

void PanelInspector::Draw()
{
	ImGui::Begin("Inspector");

	char fps[64];
	sprintf_s(fps, 64, "FPS: %f", 1.0f / App->deltaTime);
	ImGui::Text(fps);

	const char* items[] = { "Final Scene", "Albedo", "Normals", "Position", "Depth" };
	static int item_current = 0;
	ImGui::Combo("G Buffer", &item_current, items, IM_ARRAYSIZE(items));

	if (item_current == 0) {} //DrawFinalScene()
	else if (item_current == 1) {} //DrawAlbedo()
	else if (item_current == 2) {} //DrawNormals()
	else if (item_current == 3) {} //DrawPosition()
	else if (item_current == 4) {} //DrawDepth()
	ImGui::Separator();

	for (GLuint tex : M_Renderer->gbuffer.textures)
		ImGui::Image((ImTextureID)tex, ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(0,1), ImVec2(1,0));
	//ImGui::Image((ImTextureID)M_Renderer->gbuffer.depth_texture, ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth()), ImVec2(0, 1), ImVec2(1, 0));
	
	if (M_Scene->camera != nullptr)
	{
		M_Scene->camera->DrawInspector();
	}
	
	ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
	if (ImGui::BeginTabItem("Models"))
	{
		for (Model* obj : M_Scene->models)
		{
			obj->DrawInspector();
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Lights"))
	{
		if (ImGui::CollapsingHeader("Directional", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			for (int i = 0; i < M_Scene->dirLights.size(); ++i)
				M_Scene->dirLights[i]->DrawInspector(i);
		}
		if (ImGui::CollapsingHeader("Point", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			for (int i = 0; i < M_Scene->pointLights.size(); ++i)
				M_Scene->pointLights[i]->DrawInspector(i);
		}
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::End();
}
