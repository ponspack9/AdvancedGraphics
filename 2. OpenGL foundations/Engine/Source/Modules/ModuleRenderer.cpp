#include "pch.h"
#include "ModuleRenderer.h"
#include "ModuleResources.h"
#include "ModuleScene.h"

#pragma region Module overrides

ModuleRenderer* M_Renderer = nullptr;

ModuleRenderer::ModuleRenderer()
{
	this->name = "ModuleRenderer";
	LOG_DEBUG("Created module [{0}]", name);
}

ModuleRenderer::~ModuleRenderer()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}

bool ModuleRenderer::Init()
{
	u32 geometry_size = sizeof(glm::mat4) * 2;
	u32 lights_size = sizeof(glm::vec3) + sizeof(u32) + sizeof(DirectionalLight) * M_Scene->dirLights.size() + 
										  sizeof(u32) + sizeof(PointLight) * M_Scene->pointLights.size();

	// Create Uniforms Buffer
	geometry_uniform = CreateConstantBuffer(geometry_size);
	light_uniform = CreateConstantBuffer(lights_size);

	// Init GBuffer
	gbuffer.Init();

	return true;
}

bool ModuleRenderer::Update(float dt)
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Shaded model");

	// Update Camera
	M_Scene->camera->Move();

	// Geometry Pass
	GeometryPass(M_Resources->programs[App->texturedGeometryProgramIdx]);

	// Light Pass
	if (renderType == FINAL_SCENE)
		LightPass(M_Resources->programs[App->lightProgramIdx]);
	else
		RenderType();

	glPopDebugGroup();

	return true;
}

#pragma endregion

// Render to screen
void ModuleRenderer::ScenePass(Program* program)
{
	// Render on GBuffer's FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	// Clear Color and Depth
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Viewport
	glViewport(0, 0, App->displaySize.x, App->displaySize.y);

	// Set Default Flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Enable Shader
	glUseProgram(program->handle);

	// Render
	for (Model* model : M_Scene->models)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, model->position);
		transform = glm::rotate(transform, glm::radians(00.0f), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, model->scale);

		// Shader Uniforms
		BindBuffer(geometry_uniform);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, geometry_uniform.handle, 0, geometry_uniform.size);
		MapBuffer(geometry_uniform, GL_READ_ONLY);
		PushMat4(geometry_uniform, transform);
		PushMat4(geometry_uniform, M_Scene->camera->GetViewProjectionMatrix());

		// Draw Mesh
		DrawMesh(model, program);

		// Unbind Shader Uniforms
		UnmapBuffer(geometry_uniform);
	}

	// Disable Shader
	glUseProgram(0);
}

void ModuleRenderer::GeometryPass(Program* program)
{
	// Render on GBuffer's FBO
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.FBO);

	// Select Render Targets to Draw
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(ARRAY_COUNT(drawBuffers), drawBuffers);

	// Clear Color and Depth
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Viewport
	glViewport(0, 0, App->displaySize.x, App->displaySize.y);

	// Set Default Flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Enable Shader
	glUseProgram(program->handle);

	// Render
	for (Model* model : M_Scene->models)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, model->position);
		transform = glm::rotate(transform, glm::radians(00.0f), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, model->scale);

		// Shader Uniforms
		BindBuffer(geometry_uniform);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, geometry_uniform.handle, 0, geometry_uniform.size);
		MapBuffer(geometry_uniform, GL_WRITE_ONLY);
		PushMat4(geometry_uniform, transform);
		PushMat4(geometry_uniform, M_Scene->camera->GetViewProjectionMatrix());

		// Draw Mesh
		DrawMesh(model, program);

		// Unbind Shader Uniforms
		UnmapBuffer(geometry_uniform);
	}

	// Disable Shader
	glUseProgram(0);
}

void ModuleRenderer::LightPass(Program* program)
{
	// Render on Default Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	// Clear Color and Depth
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Viewport
	glViewport(0, 0, App->displaySize.x, App->displaySize.y);

	// Set Default Flags
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Enable Shader
	glUseProgram(program->handle);

	// Render
	for (Model* model : M_Scene->models)
	{
		u32 numDirLights = M_Scene->dirLights.size();
		u32 numPointLights = M_Scene->pointLights.size();

		// Shader Uniforms
		BindBuffer(light_uniform);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, light_uniform.handle, 0, light_uniform.size);
		MapBuffer(light_uniform, GL_WRITE_ONLY);
		PushVec3(light_uniform, M_Scene->camera->pos);
		PushUInt(light_uniform, numDirLights);
		for (int i = 0; i < numDirLights; ++i)
			PushData(light_uniform, M_Scene->dirLights[i], sizeof(DirectionalLight));
		PushUInt(light_uniform, numPointLights);
		for (int i = 0; i < numPointLights; ++i)
			PushData(light_uniform, M_Scene->pointLights[i], sizeof(PointLight));

		// Draw Mesh
		DrawMesh(model, program);

		// Unbind Shader Uniforms
		UnmapBuffer(light_uniform);
	}

	// Disable Shader
	glUseProgram(0);
}

void ModuleRenderer::DrawMesh(Model* model, Program* program)
{
	if (model->mesh == nullptr) 
		return;

	Mesh* mesh = model->mesh;
	for (u32 i = 0; i < mesh->submeshes.size(); ++i)
	{
		GLuint vao = FindVAO(mesh, i, program);

		glBindVertexArray(vao);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		u32 submeshMaterialIdx = model->materialIdx[i];
		Material* submeshMaterial = M_Resources->materials[submeshMaterialIdx];

		glActiveTexture(GL_TEXTURE0);
		if (submeshMaterial->albedoTexture)
			glBindTexture(GL_TEXTURE_2D, submeshMaterial->albedoTexture->handle);
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		Submesh& submesh = mesh->submeshes[i];
		glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(u64)submesh.indexOffset);

		glBindVertexArray(0);
	}
}

//--------------------------------------------------------------------
GLuint ModuleRenderer::FindVAO(Mesh* mesh, u32 submeshIndex, const Program* program)
{
	Submesh& submesh = mesh->submeshes[submeshIndex];

	// Try finding a vao for this submesh/program
	for (u32 i = 0; i < (u32)submesh.vaos.size(); ++i)
	{
		if (submesh.vaos[i].programHandle == program->handle)
			return submesh.vaos[i].handle;
	}

	//Create a new vao for this submesh/program
	GLuint vaoHandle = CreateNewVao(mesh, submesh, program);
	//Store it in the list of vaos for this submesh
	VAO vao = { vaoHandle, program->handle };
	submesh.vaos.push_back(vao);

	return vaoHandle;
}

GLuint ModuleRenderer::CreateNewVao(Mesh* mesh, Submesh& submesh, const Program* program)
{
	GLuint vaoHandle = 0;
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBufferHandle);

	// We have to link all vertex inputs attributes to attributes in the vertex buffer
	for (u32 i = 0; i < program->vertexInputLayout.attributes.size(); ++i)
	{
		bool attributeWasLinked = false;
		for (u32 j = 0; j < submesh.vertexBufferLayout.attributes.size(); ++j)
		{
			if (program->vertexInputLayout.attributes[i].location == submesh.vertexBufferLayout.attributes[j].location)
			{
				const u32 index = submesh.vertexBufferLayout.attributes[j].location;
				const u32 nComp = submesh.vertexBufferLayout.attributes[j].componentCount;
				const u32 offset = submesh.vertexBufferLayout.attributes[j].offset + submesh.vertexOffset;
				const u32 stride = submesh.vertexBufferLayout.stride;

				glVertexAttribPointer(index, nComp, GL_FLOAT, GL_FALSE, stride, (void*)(u64)offset);
				glEnableVertexAttribArray(index);

				attributeWasLinked = true;
				break;
			}
		}
		assert(attributeWasLinked); // The submesh should provide an attribute for each vertex input
	}
	glBindVertexArray(0);

	return vaoHandle;
}

void ModuleRenderer::RenderType()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.FBO);

	glReadBuffer(GL_COLOR_ATTACHMENT0 + renderType - 1);
	glBlitFramebuffer(0, 0, App->displaySize.x, App->displaySize.y, 0, 0, App->displaySize.x, App->displaySize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}