#pragma once

class Model
{
public:
    Model();
    ~Model();

    void DrawInspector();

public:
    Mesh* mesh;
    std::vector<u32> materialIdx;
    std::string name;

    vec3 position = { 0.0f, 0.0f, 0.0f };
    vec3 scale = { 1.0f, 1.0f, 1.0f };

    u32 localParams_offset;
    u32 localParams_size;

    // ImGui things
    std::string index = "0";
};
