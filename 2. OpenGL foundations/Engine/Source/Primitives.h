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

//Submesh quadSubmesh;
    //quadSubmesh.vertices = {
    //     -0.5f, -0.5f, 0.0f,   0.0f,0.0f,
    //      0.5f, -0.5f, 0.0f,   1.0f,0.0f,
    //      0.5f,  0.5f, 0.0f,   1.0f,1.0f,
    //     -0.5f,  0.5f, 0.0f,   0.0f,1.0f };
    //quadSubmesh.indices = {
    //    0, 1, 2,
    //    0, 2, 3
    //};
    //quadSubmesh.vertexOffset = 0;
    //quadSubmesh.indexOffset = 0;
    //quadSubmesh.vertexBufferLayout.attributes = { { 0,3,0 },{ 1,2,3 * sizeof(float) } };
    //quadSubmesh.vertexBufferLayout.stride = 20;

    //Mesh quadMesh;
    //quadMesh.submeshes.push_back(quadSubmesh);
    //

    //// VBO
    //glGenBuffers(1, &quadMesh.vertexBufferHandle);
    //glBindBuffer(GL_ARRAY_BUFFER, quadMesh.vertexBufferHandle);
    //glBufferData(GL_ARRAY_BUFFER, quadMesh.submeshes[0].vertices.size() * sizeof(float), &quadMesh.submeshes[0].vertices[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //// EBO
    //glGenBuffers(1, &quadMesh.indexBufferHandle);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh.indexBufferHandle);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadMesh.submeshes[0].indices.size() * sizeof(u32), &quadMesh.submeshes[0].indices[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    //// VAO
    //// Element 0, has 3 components, that are floats, no need to normalize, 
    ////GLuint vao = 0;
    //// TODO: VAO
    //glGenVertexArrays(1, &App->vao);
    //glBindVertexArray(App->vao);
    //glBindBuffer(GL_ARRAY_BUFFER, quadMesh.vertexBufferHandle);

    //for (u32 i = 0; i < quadMesh.submeshes.size(); ++i)
    //{
    //    Submesh* submesh = &quadMesh.submeshes[i];
    //    u8 stride = submesh->vertexBufferLayout.stride;

    //    submesh->vaos.push_back({ App->vao,App->texturedGeometryProgramIdx });
    //    for (VertexBufferAttribute attribute : submesh->vertexBufferLayout.attributes)
    //    {
    //        glVertexAttribPointer(attribute.location, attribute.componentCount, GL_FLOAT, GL_FALSE, stride, (void*)attribute.offset);
    //        glEnableVertexAttribArray(attribute.location);

    //    }
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMesh.indexBufferHandle);
    //    glBindVertexArray(0);
    //}

    //meshes.push_back(quadMesh);
    //
    //Program& texturedGeometryProgram = M_Resources->programs[App->texturedGeometryProgramIdx];

    //App->programUniformTexture = glGetUniformLocation(texturedGeometryProgram.handle, "uTexture");

    //// Textures
    //App->diceTexIdx = M_Resources->LoadTexture2D("dice.png");
    //App->whiteTexIdx = M_Resources->LoadTexture2D("color_white.png");
    //App->blackTexIdx = M_Resources->LoadTexture2D("color_black.png");
    //App->normalTexIdx = M_Resources->LoadTexture2D("color_normal.png");
    //App->magentaTexIdx = M_Resources->LoadTexture2D("color_magenta.png");

    //Model quad;
    //quad.meshIdx = 0;
    //quad.materialIdx.push_back(0);

    //models.push_back(quad);

    //Material material;
    //material.albedoTextureIdx = App->diceTexIdx;

    //materials.push_back(material);