#pragma once
#include "Module.h"
#include <Utils/GBuffer.h>

class ModuleRenderer :  public Module
{
public:
	ModuleRenderer();
	~ModuleRenderer();

	bool Init() override;
	bool Update(float dt) override;

	void ScenePass(Program* program);

	void GeometryPass(Program* program);
	void LightPass(Program* program);

	void DrawMesh(Model* mesh, Program* program);

private:
	GLuint FindVAO(Mesh* mesh, u32 submeshIndex, const Program* program);
	GLuint CreateNewVao(Mesh* mesh, Submesh& submesh, const Program* program);

public:
	int maxUniformBufferSize = 0;
	int uniformBlockAlignment = 0;
	GLuint bufferHandle = 0;

	GBuffer gbuffer;
};

extern ModuleRenderer* M_Renderer;
