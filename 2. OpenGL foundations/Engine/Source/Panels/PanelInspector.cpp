#include "pch.h"
#include "PanelInspector.h"
#include <ModuleScene.h>
#include <GameObjects/GameObject.h>

void PanelInspector::Draw()
{
	GameObject* obj = M_Scene->selected;
	if (obj == nullptr) return;

	ImGui::Begin("Inspector");
	for (Component* component : obj->components)
	{
		component->DrawInspector();
	}

	ImGui::End();
}
