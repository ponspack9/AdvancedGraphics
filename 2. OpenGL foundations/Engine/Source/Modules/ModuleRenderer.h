#pragma once
#include "Module.h"
class ModuleRenderer :  public Module
{
public:

	ModuleRenderer();
	~ModuleRenderer();

	bool Init() override;
	bool Update(float dt) override;

public:

	GLuint FindVAO(Mesh* mesh, u32 submeshIndex, const Program* program);
	GLuint CreateNewVao(Mesh* mesh, Submesh& submesh, const Program* program);


	int maxUniformBufferSize = 0;
	int uniformBlockAlignment = 0;
	GLuint bufferHandle = 0;

};

extern ModuleRenderer* M_Renderer;
