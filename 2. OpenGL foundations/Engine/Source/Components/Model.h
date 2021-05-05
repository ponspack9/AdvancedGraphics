#pragma once
#include <Components/Component.h>

class Model : public Component
{
public:

    Mesh* mesh;
    std::vector<u32> materialIdx;
    std::string name;

    Model();
    ~Model();

    void DrawInspector() override;

};