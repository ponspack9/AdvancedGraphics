#pragma once
#include "Module.h"

class GBuffer;

class ModuleRenderer :  public Module
{
public:

	ModuleRenderer();
	~ModuleRenderer();

	bool Init() override;
	bool Update(float dt) override;

	void GeometryPass();
	void LightPass();

	void DrawMesh(Program* program, Model* model, glm::mat4 viewProjection);

	GLuint FindVAO(Mesh* mesh, u32 submeshIndex, const Program* program);
	GLuint CreateNewVao(Mesh* mesh, Submesh& submesh, const Program* program);


public:
	int maxUniformBufferSize = 0;
	int uniformBlockAlignment = 0;
	GLuint bufferHandle = 0;

	GBuffer gbuffer;
};

extern ModuleRenderer* M_Renderer;
