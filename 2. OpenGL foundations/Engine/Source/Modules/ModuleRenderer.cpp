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
	// Create Uniforms Buffer
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBufferSize);
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBlockAlignment);
	uniforms = CreateConstantBuffer(maxUniformBufferSize);

	// Init GBuffer
	gbuffer.Init();

	return true;
}

bool ModuleRenderer::Update(float dt)
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Shaded model");

	// Update Camera
	M_Scene->camera->Move();

	// Load Uniforms Buffer
	LoadUniforms();

	// --- Render
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.FBO); // Render on GBuffer's FBO
	GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0, // Light
		GL_COLOR_ATTACHMENT1, // Albedo
		GL_COLOR_ATTACHMENT2, // Normals
		GL_COLOR_ATTACHMENT3, // Position
		GL_COLOR_ATTACHMENT4, // Depth
	};
	glDrawBuffers(ARRAY_COUNT(drawBuffers), drawBuffers);

	// Clear Screen & Set Viewport
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, App->displaySize.x, App->displaySize.y);

	// Set Default Flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// --- Geometry Pass
	GeometryPass(M_Resources->programs[App->texturedGeometryProgramIdx]);

	// --- Light Pass
	LightPass(M_Resources->programs[App->lightProgramIdx]);

	// --- Render to Screen
	RenderType();

	glPopDebugGroup();
	return true;
}

#pragma endregion

void ModuleRenderer::GeometryPass(Program* program)
{
	// Enable Shader
	glUseProgram(program->handle);

	// Render
	for (Model* model : M_Scene->models)
	{
		//Bind Local Params
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, uniforms.handle, model->localParams_offset, sizeof(glm::mat4) * 2);

		// Draw Mesh
		Mesh* mesh = model->mesh;
		if (mesh == nullptr) continue;
		for (u32 i = 0; i < mesh->submeshes.size(); ++i)
		{
			// VAO
			GLuint vao = FindVAO(mesh, i, program);
			glBindVertexArray(vao);

			// Material
			u32 submeshMaterialIdx = model->materialIdx[i];
			Material* submeshMaterial = M_Resources->materials[submeshMaterialIdx];

			// Pass Textures to Uniform
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, submeshMaterial->albedoTexture->handle);
			glUniform1i(App->programUniformTexture, 0);

			// Draw
			Submesh& submesh = mesh->submeshes[i];
			glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(u64)submesh.indexOffset);

			glBindVertexArray(0);
		}
	}
	glUseProgram(0); // Disable Shader
}

void ModuleRenderer::LightPass(Program* program)
{
	// Enable Shader
	glUseProgram(program->handle);

	// Pass the Textures in Order
	glUniform1i(glGetUniformLocation(program->handle, "oAlbedo"),	0);
	glUniform1i(glGetUniformLocation(program->handle, "oNormal"),	1);
	glUniform1i(glGetUniformLocation(program->handle, "oPosition"), 2);
	glUniform1i(glGetUniformLocation(program->handle, "oDepth"),	3);

	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[1]);
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[2]);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[3]);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[4]);

	// Draw on Final Color Attachment
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glDepthMask(false);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniforms.handle, globalParams_offset, globalParams_size);

	glDepthMask(true);
	glUseProgram(0); // Disable Shader
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	glReadBuffer(GL_COLOR_ATTACHMENT0 + renderType);
	glBlitFramebuffer(0, 0, App->displaySize.x, App->displaySize.y, 0, 0, App->displaySize.x, App->displaySize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void ModuleRenderer::LoadUniforms()
{
	MapBuffer(uniforms, GL_WRITE_ONLY);

	//Global params
	globalParams_offset = uniforms.head;
	PushVec3(uniforms, M_Scene->camera->pos);
	PushUInt(uniforms, M_Scene->lights.size());

	for (Light* light : M_Scene->lights)
	{
		AlignHead(uniforms, sizeof(vec4));

		PushUInt(uniforms, (u32)light->type);
		PushUInt(uniforms, (u32)light->range);
		PushVec3(uniforms, light->color);
		PushVec3(uniforms, light->direction);
		PushVec3(uniforms, light->position);
	}
	globalParams_size = uniforms.head - globalParams_offset;

	//Local params
	for (Model* model : M_Scene->models)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, model->position);
		transform = glm::rotate(transform, glm::radians(00.0f), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, model->scale);

		AlignHead(uniforms, uniformBlockAlignment);

		model->localParams_offset = uniforms.head;
		PushMat4(uniforms, transform);
		PushMat4(uniforms, M_Scene->camera->GetViewProjectionMatrix());
		model->localParams_size = uniforms.head - model->localParams_offset;
	}

	UnmapBuffer(uniforms);
}