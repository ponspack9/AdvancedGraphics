#include "pch.h"
#include "PanelInspector.h"
#include <ModuleScene.h>

void PanelInspector::Draw()
{
	ImGui::Begin("Inspector");

	const char* items[] = { "Final Scene", "Albedo", "Normals", "Position", "Depth" };
	static int item_current = 0;
	ImGui::Combo("G Buffer", &item_current, items, IM_ARRAYSIZE(items));

	if (item_current == 0) {} //DrawFinalScene()
	else if (item_current == 1) {} //DrawAlbedo()
	else if (item_current == 2) {} //DrawNormals()
	else if (item_current == 3) {} //DrawPosition()
	else if (item_current == 4) {} //DrawDepth()
	ImGui::Separator();

	if (M_Scene->camera != nullptr)
	{
		M_Scene->camera->DrawInspector();
		ImGui::Separator();
	}
	
	for (Model* obj : M_Scene->models)
	{
		obj->DrawInspector();
	}

	ImGui::End();
}
