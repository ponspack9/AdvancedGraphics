#include "pch.h"
#include "PanelInspector.h"
#include <ModuleScene.h>
#include <ModuleRenderer.h>

void PanelInspector::Draw()
{
	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize);

	char fps[64];
	sprintf_s(fps, 64, "FPS: %f", 1.0f / App->deltaTime);
	ImGui::Text(fps);

	const char* items[] = { "Final Scene", "Albedo", "Normals", "Position", "Depth" };
	static int item_current = 0;
	static int size = ImGui::GetContentRegionAvailWidth();

	ImGui::Combo("Textures", &item_current, items, IM_ARRAYSIZE(items));
	M_Renderer->renderType = item_current;
	ImGui::Separator();

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
			for (int i = 0; i < M_Scene->lights.size(); ++i)
			{
				if (M_Scene->lights[i]->type == Light::LightType::DIRECTIONAL)
					M_Scene->lights[i]->DrawInspector(Light::LightType::DIRECTIONAL, i);
			}
		}
		if (ImGui::CollapsingHeader("Point", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			for (int i = 0; i < M_Scene->lights.size(); ++i)
			{
				if (M_Scene->lights[i]->type == Light::LightType::POINT)
					M_Scene->lights[i]->DrawInspector(Light::LightType::POINT, i);
			}
		}
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();

	ImGui::End();
}
