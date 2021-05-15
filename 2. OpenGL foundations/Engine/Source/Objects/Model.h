#pragma once

class Model
{
public:

    Mesh* mesh;
    std::vector<u32> materialIdx;
    std::string name;

    vec3 position = { 0.0f, 0.0f, 0.0f };

    Model();
    ~Model();

    void DrawInspector();
};