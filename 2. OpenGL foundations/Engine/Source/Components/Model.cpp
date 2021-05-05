#include "pch.h"

Model::Model() : Component(Component::Type::Model, nullptr)
{

}

Model::~Model()
{
}

void Model::DrawInspector()
{
    ImGui::BeginChild(name.c_str());
    ImGui::Text(name.c_str());
    ImGui::EndChild();
}
