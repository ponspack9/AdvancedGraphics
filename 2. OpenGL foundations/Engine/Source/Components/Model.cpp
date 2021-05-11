#include "pch.h"
#include <ModuleResources.h>

#define TEX_SIZE 256

Model::Model() : Component(Component::Type::Model, nullptr)
{

}

Model::~Model()
{
}

void Model::DrawInspector()
{
    ImGui::BeginChild(name.c_str());

    if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth))
        ImGui::Image((ImTextureID)M_Resources->textures[0]->handle, ImVec2(TEX_SIZE, TEX_SIZE));
    
    ImGui::EndChild();
}
