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

    static u32 plane;
    static u32 cube;
    static u32 sphereLow;
    static u32 sphereHigh;
    static std::vector<u32> primitives;
	
};

u32 Primitives::plane = 0;
u32 Primitives::cube = 0;
u32 Primitives::sphereLow = 0;
u32 Primitives::sphereHigh = 0;
std::vector<u32> Primitives::primitives = std::vector<u32>();
