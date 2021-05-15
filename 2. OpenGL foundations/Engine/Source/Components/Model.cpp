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
