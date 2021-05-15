#pragma once
#include "Module.h"
class ModuleRenderer :  public Module
{
public:

	ModuleRenderer();
	~ModuleRenderer();

	bool Update(float dt) override;
public:
	void DrawMesh(Program* program, Model* model, glm::mat4 viewProjection);

	GLuint FindVAO(Mesh* mesh, u32 submeshIndex, const Program* program);
	GLuint CreateNewVao(Mesh* mesh, Submesh& submesh, const Program* program);
};

extern ModuleRenderer* M_Renderer;
