#include "pch.h"
#include <ModuleResources.h>

#define TEX_SIZE 256

Model::Model()
{

}

Model::~Model()
{
}

void Model::DrawInspector()
{
    ImGui::BeginChild(name.c_str());

    ImGui::Text("Position");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat("x##model_pos", &position.x);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat("y##model_pos", &position.y);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat("z##model_pos", &position.z);

    if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_SpanAvailWidth))
    {
        for (int i = 0; i < materialIdx.size(); ++i)
        {
            int mat = materialIdx[i];
            ImGui::Text(M_Resources->materials[mat]->name.c_str());
            std::vector<Texture*> materialTextures = M_Resources->materials[mat]->GetTextures();

            for (Texture* texture : materialTextures)
            {
                if (texture != nullptr)
                {
                    ImGui::Text(texture->filepath.c_str());
                    ImGui::Image((ImTextureID)texture->handle, ImVec2(TEX_SIZE, TEX_SIZE));
                }

            }
        }
    }
    ImGui::EndChild();
}
