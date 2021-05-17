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

	if (M_Scene->camera != nullptr)
	{
		M_Scene->camera->DrawInspector();
	}
	
	ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
	if (ImGui::BeginTabItem("G Buffer"))
	{
		const char* items[] = { "Albedo", "Normals", "Position", "Depth" };
		static int item_current = 0;
		static int size = ImGui::GetContentRegionAvailWidth();

		ImGui::Combo("Textures", &item_current, items, IM_ARRAYSIZE(items));
		if (item_current == 0)		ImGui::Image((ImTextureID)M_Renderer->gbuffer.textures[0], ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
		else if (item_current == 1) ImGui::Image((ImTextureID)M_Renderer->gbuffer.textures[1], ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
		else if (item_current == 2) ImGui::Image((ImTextureID)M_Renderer->gbuffer.textures[2], ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
		else if (item_current == 3) ImGui::Image((ImTextureID)M_Renderer->gbuffer.textures[3], ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndTabItem();
	}
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
