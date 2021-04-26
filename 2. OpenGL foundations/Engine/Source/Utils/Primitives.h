#pragma once
#include <ModuleResources.h>
#include <engine.h>
#include <vector>


class Primitives
{
public:

    static void Init()
    {
        plane = M_Resources->LoadModel("plane.obj");
        cube = M_Resources->LoadModel("cube.obj");
        sphereLow = M_Resources->LoadModel("Low_sphere.obj");
        sphereHigh = M_Resources->LoadModel("High_sphere.obj");

        primitives.push_back(plane);
        primitives.push_back(cube);
        primitives.push_back(sphereLow);
        primitives.push_back(sphereHigh);
    }

public:

    static Model* plane;
    static Model* cube;
    static Model* sphereLow;
    static Model* sphereHigh;
    static std::vector<Model*> primitives;
	
};

Model* Primitives::plane = 0;
Model* Primitives::cube = 0;
Model* Primitives::sphereLow = 0;
Model* Primitives::sphereHigh = 0;
std::vector<Model*> Primitives::primitives = std::vector<Model*>();
