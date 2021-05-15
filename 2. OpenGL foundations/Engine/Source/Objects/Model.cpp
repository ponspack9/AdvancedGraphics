#include "pch.h"
#include <ModuleResources.h>
#include <ModuleScene.h>

#define TEX_SIZE 256

Model::Model()
{
}

Model::~Model()
{
}

void Model::DrawInspector()
{
    //ImGui::BeginChild(name.c_str());
    
    ImGui::Text("Position");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string("x##model_pos" + index).c_str(), &position.x);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string("y##model_pos" + index).c_str(), &position.y);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string("z##model_pos" + index).c_str(), &position.z);

    ImGui::Text("Scale");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string("x##scale" + index).c_str(), &scale.x);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string("y##scale" + index).c_str(), &scale.y);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(60.0f);
    ImGui::DragFloat(std::string("z##scale" + index).c_str(), &scale.z);

    if (ImGui::CollapsingHeader((name + index).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth))
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
    //ImGui::EndChild();
}
