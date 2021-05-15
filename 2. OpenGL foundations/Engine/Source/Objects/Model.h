#pragma once

class Model
{
public:

    Mesh* mesh;
    std::vector<u32> materialIdx;
    std::string name;

    Model();
    ~Model();

    void DrawInspector();
};